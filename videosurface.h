#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include "multiparturstatus.h"

#include <opencv2/opencv.hpp>
#include <opencv2/wechat_qrcode.hpp>

#include <bc-ur.hpp>

#include <QLabel>
#include <QAbstractVideoSurface>

#include <string>

struct UrCompare
{
    int GetUrSeq(const std::string& str) const
    {
        const auto i = std::find(str.begin(), str.end(), '/') + 1;
        const auto j = std::find(i, str.end(), '-');
        const std::string seq = str.substr(i - str.begin(), j-i);
        return std::stoi(seq);
    }

    bool operator() (const std::string& lhs, const std::string& rhs) const
    {
        const bool hasLhsSeqPart = std::count(lhs.begin(), lhs.end(), '/') == 2;
        const bool hasRhsSeqPart = std::count(rhs.begin(), rhs.end(), '/') == 2;
        if (!hasLhsSeqPart || !hasRhsSeqPart)
        {
            return lhs < rhs;
        }
        return GetUrSeq(lhs) < GetUrSeq(rhs);
    }
};

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    VideoSurface(QLabel*, MultipartUrStatus*);

    void pauseScanning();
    void resumeScanning();

    bool present(const QVideoFrame &frame) override;
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const override;

public slots:
    void clear();

signals:
    void newResult(const std::set<std::string, UrCompare>&);

private:
    cv::Mat ToMat(const QVideoFrame&) const;
    cv::Mat ToMat(const QImage&) const;

    QImage ToQImage(const cv::Mat&) const;
    QImage ToQImage(const QVideoFrame&) const;

    void FindQrEncodedUrs(cv::Mat&);

    bool IsMultiPartUr(const std::string&) const;
    bool IsSinglePartUr(const std::string&) const;

    QLabel* m_Label;
    MultipartUrStatus* m_MultipartUrStatus;

    QScopedPointer<cv::wechat_qrcode::WeChatQRCode> m_Detector;
    QScopedPointer<ur::URDecoder> m_UrDecoder;

    std::set<std::string, UrCompare> m_DecodedQrData;

    bool m_Paused = false;
    bool m_CommitedToMultipart = false;
};

#endif // VIDEOSURFACE_H
