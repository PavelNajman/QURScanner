#include "videosurface.h"

#include <QVBoxLayout>
#include <QPixmap>

namespace urscanner
{

VideoSurface::VideoSurface(QObject* parent)
    : QAbstractVideoSurface(parent)
{
    m_Detector.reset(new cv::wechat_qrcode::WeChatQRCode("detect.prototxt", "detect.caffemodel", "sr.prototxt", "sr.caffemodel"));
}

bool VideoSurface::present(const QVideoFrame &frame)
{
    if (!supportedPixelFormats().contains(frame.pixelFormat()))
    {
        setError(IncorrectFormatError);
        return false;
    }

    QVideoFrame frametodraw(frame);
    if(!frametodraw.map(QAbstractVideoBuffer::ReadOnly))
    {
       setError(ResourceError);
       return false;
    }    

    auto mat = ToMat(frametodraw);

    frametodraw.unmap();

    if (!m_Paused)
    {
        FindQrEncodedUrs(mat);
    }

    QImage image = ToQImage(mat);

    QPixmap p = QPixmap::fromImage(image);

    emit NewFrame(p);

    return true;
}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType) const
{
    return {QVideoFrame::Format_RGB24};
}

void VideoSurface::PauseScanning()
{
    m_Paused = true;
}

void VideoSurface::ResumeScanning()
{
    m_Paused = false;
}

void VideoSurface::CancelMultipartUrScan()
{
    m_CommitedToMultipart = false;
}

cv::Mat VideoSurface::ToMat(const QVideoFrame& frame) const
{
    cv::Mat result;
    switch(frame.pixelFormat())
    {
        case QVideoFrame::Format_RGB24:
        {
            const cv::Mat view(frame.height(), frame.width(), CV_8UC3, (void *)frame.bits(), frame.bytesPerLine());
            cv::cvtColor(view, result, cv::COLOR_RGB2BGR);
            break;
        }
        default:
            break;
    }
    return result;
}

QImage VideoSurface::ToQImage(const cv::Mat& image) const
{
    QImage result;
    switch(image.type())
    {
        case CV_8UC3:
        {
            cv::Mat mat;
            cvtColor(image, mat, cv::COLOR_BGR2BGRA);
            QImage view(mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_ARGB32);
            result = view.copy();
            break;
        }
        default:
            break;
    }
    return result;
}

void VideoSurface::FindQrEncodedUrs(cv::Mat& img)
{
    std::vector<cv::Mat> points;
    const auto res = m_Detector->detectAndDecode(img, points);
    for (size_t i = 0; i < points.size(); ++i)
    {
        cv::rectangle(img, cv::Point(points[i].row(0)), cv::Point(points[i].row(2)), cv::Scalar(255, 0, 0));

        if (m_CommitedToMultipart && IsSinglePartUr(res[i]))
        {   // skip single-part ur when multi-part ur decoding is already in progress
            continue;
        }

        if (!m_CommitedToMultipart)
        {   // when no decoding is in progress, reset decoder
            m_UrDecoder.reset(new ur::URDecoder());
            m_DecodedQrData.clear();
        }

        if (m_UrDecoder->receive_part(res[i]))
        {   // when the part was successfully received, we store the encoded data and commit to decoding
            m_CommitedToMultipart = true;
            m_DecodedQrData.emplace(res[i]);
        }

        if (m_UrDecoder->is_complete())
        {   // when the decoding is complete, emit signal with the result
            m_CommitedToMultipart = false;
            emit NewResult(m_UrDecoder->result_ur(), m_DecodedQrData);
        }

        if (m_CommitedToMultipart)
        {   // when multi-part ur decoding is in progress, display and update progress
            emit UpdateMultipartUrProgress(m_UrDecoder->expected_part_count(), m_UrDecoder->received_part_indexes());
        }
    }
}

} // namespace urscanner
