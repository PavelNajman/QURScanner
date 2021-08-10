#include "videosurface.h"
#include <QVBoxLayout>

#include <algorithm>
#include <regex>

VideoSurface::VideoSurface(QLabel* label, MultipartUrStatus* status)
    : m_Label(label)
    , m_MultipartUrStatus(status)
{
    m_Detector.reset(new cv::wechat_qrcode::WeChatQRCode("detect.prototxt", "detect.caffemodel", "sr.prototxt", "sr.caffemodel"));

    connect(m_MultipartUrStatus, SIGNAL(Cancel()), this, SLOT(clear()));

    m_MultipartUrStatus->hide();
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
    m_Label->setPixmap(p.scaled(m_Label->size() - QSize(0, m_MultipartUrStatus->height()), Qt::KeepAspectRatio));
    m_Label->update();

    m_MultipartUrStatus->setMinimumWidth(m_Label->pixmap()->width());

    return true;
}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType) const
{
    return {QVideoFrame::Format_RGB24};
}

void VideoSurface::pauseScanning()
{
    m_Paused = true;
}

void VideoSurface::resumeScanning()
{
    m_Paused = false;
}

void VideoSurface::clear()
{
    m_CommitedToMultipart = false;
    m_DecodedQrData.clear();
    m_MultipartUrStatus->hide();
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

cv::Mat VideoSurface::ToMat(const QImage& image) const
{
    cv::Mat result;
    switch(image.format())
    {
        case QImage::Format_RGB888:
        {
            cv::Mat view(image.height(), image.width(), CV_8UC3, (void *)image.constBits(), image.bytesPerLine());
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

QImage VideoSurface::ToQImage(const QVideoFrame& frame) const
{
    switch(frame.pixelFormat())
    {
        case QVideoFrame::Format_RGB24:
            return QImage(frame.bits(), frame.width(), frame.height(), frame.bytesPerLine(), QImage::Format_RGB888);
        default:
            break;
    }
    return {};
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
        }

        if (m_UrDecoder->receive_part(res[i]))
        {   // when the part was successfully received, we store the encoded data and commit to decoding
            m_CommitedToMultipart = true;
            m_DecodedQrData.emplace(res[i]);
        }

        if (m_UrDecoder->is_complete())
        {   // when the decoding is complete, emit signal with the result
            m_CommitedToMultipart = false;
            emit newResult(m_DecodedQrData);
        }

        if (m_CommitedToMultipart)
        {   // when multi-part ur decoding is in progress, display and update progress
            m_MultipartUrStatus->SetNumParts(m_UrDecoder->expected_part_count());
            m_MultipartUrStatus->SetCompletedParts(m_UrDecoder->received_part_indexes());
            m_MultipartUrStatus->setMinimumHeight(10);
            m_MultipartUrStatus->show();
            m_MultipartUrStatus->update();
        }
    }
}

bool VideoSurface::IsMultiPartUr(const std::string& str) const
{
    const auto multiPartUrRegex = std::regex("ur:[a-zA-Z0-9-]+/[1-9][0-9]*-[1-9][0-9]*/[a-z]+");
    return std::regex_match(str, multiPartUrRegex);
}

bool VideoSurface::IsSinglePartUr(const std::string& str) const
{
    const auto singlePartUrRegex = std::regex("ur:[a-zA-Z0-9-]+/[a-z]+");
    return std::regex_match(str, singlePartUrRegex);
}


