#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <set>
#include <string>

#include <QAbstractVideoSurface>

#include <opencv2/opencv.hpp>
#include <opencv2/wechat_qrcode.hpp>

#include <bc-ur.hpp>

#include "urutils.h"

namespace urscanner
{

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit VideoSurface(QObject* parent = nullptr);

    bool present(const QVideoFrame &frame) override;
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const override;

public slots:
    void PauseScanning();
    void ResumeScanning();
    void CancelMultipartUrScan();

signals:
    void NewFrame(const QPixmap&);
    void NewResult(const ur::UR&, const std::set<std::string, UrCompare>&);
    void UpdateMultipartUrProgress(const size_t, const std::set<size_t>&);

private:
    cv::Mat ToMat(const QVideoFrame&) const;
    QImage ToQImage(const cv::Mat&) const;

    void FindQrEncodedUrs(cv::Mat&);

    QScopedPointer<cv::wechat_qrcode::WeChatQRCode> m_Detector;
    QScopedPointer<ur::URDecoder> m_UrDecoder;

    std::set<std::string, UrCompare> m_DecodedQrData;

    bool m_Paused = false;
    bool m_CommitedToMultipart = false;
};

} // namespace urscanner

#endif // VIDEOSURFACE_H
