/*
 *  Copyright (c) 2021 Pavel Najman
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to dea in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <set>
#include <string>

#include <QAbstractVideoSurface>

#include <opencv2/opencv.hpp>
#include <opencv2/wechat_qrcode.hpp>

#include <bc-ur.hpp>

#include "urutils.h"

namespace qurscanner
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

} // namespace qurscanner

#endif // VIDEOSURFACE_H
