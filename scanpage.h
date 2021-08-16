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

#ifndef SCANPAGE_H
#define SCANPAGE_H

#include "urutils.h"
#include "videosurface.h"
#include "multiparturstatus.h"

#include <bc-ur.hpp>

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QList>
#include <QCamera>
#include <QCameraInfo>

namespace qurscanner
{

class ScanPage : public QWidget
{
    Q_OBJECT
public:
    explicit ScanPage(QWidget* parent = nullptr);

public slots:
    void NewScan();

signals:
    void NewResult(const ur::UR&, const std::set<std::string, UrCompare>&);

private slots:
    void SetCamera(const QCameraInfo& cameraInfo);
    void SelectCamera(const int index);
    void DisplayCameraError();

    void ShowNewFrame(const QPixmap&);

private:
    const QSize PREFERRED_RESOLUTION = QSize(800, 600);

    void InitAvailableCamerasComboBox();
    void UpdateCaptureMode();

    QComboBox* m_AvailableCamerasComboBox;
    VideoSurface* m_VideoSurface;
    QLabel* m_VideoFrameLabel;
    MultipartUrStatus* m_MultipartUrStatus;

    QList<QCameraInfo> m_AvailableCameras = QCameraInfo::availableCameras();
    QScopedPointer<QCamera> m_Camera;
};

} // namespace qurscanner

#endif // SCANPAGE_H
