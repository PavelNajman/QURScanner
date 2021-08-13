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

#include "scanpage.h"

#include <QVBoxLayout>
#include <QMediaRecorder>
#include <QMessageBox>

namespace urscanner
{

ScanPage::ScanPage(QWidget *parent)
    : QWidget(parent)
    , m_AvailableCamerasComboBox(new QComboBox)
    , m_VideoSurface(new VideoSurface(this))
    , m_VideoFrameLabel(new QLabel)
    , m_MultipartUrStatus(new MultipartUrStatus)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_AvailableCamerasComboBox);
    layout->addWidget(m_VideoFrameLabel);
    layout->addWidget(m_MultipartUrStatus);

    connect(m_AvailableCamerasComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SelectCamera(int)));

    connect(m_VideoSurface, SIGNAL(NewFrame(QPixmap)), this, SLOT(ShowNewFrame(QPixmap)));
    connect(m_VideoSurface, SIGNAL(UpdateMultipartUrProgress(size_t, std::set<size_t>)), m_MultipartUrStatus, SLOT(UpdateProgress(size_t, std::set<size_t>)));
    connect(m_VideoSurface, SIGNAL(UpdateMultipartUrProgress(size_t, std::set<size_t>)), m_MultipartUrStatus, SLOT(update()));
    connect(m_VideoSurface, SIGNAL(UpdateMultipartUrProgress(size_t, std::set<size_t>)), m_MultipartUrStatus, SLOT(show()));
    connect(m_VideoSurface, SIGNAL(NewResult(const ur::UR&, std::set<std::string,UrCompare>)), this, SIGNAL(NewResult(const ur::UR&, std::set<std::string,UrCompare>)));
    connect(m_VideoSurface, SIGNAL(NewResult(const ur::UR&, std::set<std::string,UrCompare>)), m_VideoSurface, SLOT(PauseScanning()));

    connect(m_MultipartUrStatus, SIGNAL(Cancel()), m_VideoSurface, SLOT(CancelMultipartUrScan()));
    connect(m_MultipartUrStatus, SIGNAL(Cancel()), m_MultipartUrStatus, SLOT(hide()));

    InitAvailableCamerasComboBox();

    m_VideoFrameLabel->setAlignment(Qt::AlignCenter);

    m_MultipartUrStatus->hide();
}

void ScanPage::NewScan()
{
    m_VideoSurface->CancelMultipartUrScan();
    m_VideoSurface->ResumeScanning();
    m_MultipartUrStatus->hide();
}

void ScanPage::SetCamera(const QCameraInfo& cameraInfo)
{
    if (!m_Camera.isNull())
    {
        m_Camera->stop();
        m_Camera->unload();
    }

    m_Camera.reset(new QCamera(cameraInfo));

    connect(m_Camera.data(), QOverload<QCamera::Error>::of(&QCamera::error), this, &ScanPage::DisplayCameraError);

    m_Camera->setViewfinder(m_VideoSurface);

    m_Camera->load();

    UpdateCaptureMode();
    const auto videoCapture = new QMediaRecorder(m_Camera.data());
    if (videoCapture->supportedResolutions().contains(PREFERRED_RESOLUTION))
    {
        QCameraViewfinderSettings settings;
        settings.setResolution(PREFERRED_RESOLUTION);
        m_Camera->setViewfinderSettings(settings);
    }
    else if (!videoCapture->supportedResolutions().empty())
    {   // find resolution that is closest to the preferred
        const auto prefferedResolutionArea = PREFERRED_RESOLUTION.width() * PREFERRED_RESOLUTION.height();
        auto nearestResolution = videoCapture->supportedResolutions().front();
        auto nearestResolutionArea = nearestResolution.width() * nearestResolution.height();
        for (const auto& resolution : videoCapture->supportedResolutions())
        {
            const auto resolutionArea = resolution.width() * resolution.height();
            if (std::abs(resolutionArea - prefferedResolutionArea) < std::abs(nearestResolutionArea - prefferedResolutionArea))
            {
                nearestResolution = resolution;
                nearestResolutionArea = resolutionArea;
            }
        }
        QCameraViewfinderSettings settings;
        settings.setResolution(nearestResolution);
        m_Camera->setViewfinderSettings(settings);
    }

    m_Camera->start();
}

void ScanPage::SelectCamera(const int index)
{
    SetCamera(m_AvailableCameras[index]);
}

void ScanPage::ShowNewFrame(const QPixmap &p)
{
    m_VideoFrameLabel->setPixmap(p.scaled(m_VideoFrameLabel->size(), Qt::KeepAspectRatio));
    m_VideoFrameLabel->update();
}

void ScanPage::InitAvailableCamerasComboBox()
{
    // init available cameras combo box
    for (const auto& camera : m_AvailableCameras)
    {
        m_AvailableCamerasComboBox->addItem(camera.description() + " - " + camera.deviceName());
    }

    // set default camera as the current camera
    for (int i = 0; i < m_AvailableCameras.size(); ++i)
    {
        if (m_AvailableCameras[i] == QCameraInfo::defaultCamera())
        {
            m_AvailableCamerasComboBox->setCurrentIndex(i);
        }
    }
}

void ScanPage::UpdateCaptureMode()
{
    QCamera::CaptureModes captureMode = QCamera::CaptureVideo;
    if (m_Camera->isCaptureModeSupported(captureMode))
    {
        m_Camera->setCaptureMode(captureMode);
    }
}

void ScanPage::DisplayCameraError()
{
    QMessageBox::warning(this, tr("Camera Error"), m_Camera->errorString());
}

} // namespace urscanner
