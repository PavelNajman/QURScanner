/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "camera.h"
#include "ui_camera.h"

#include <QMediaRecorder>
#include <QCameraViewfinder>
#include <QMessageBox>

Q_DECLARE_METATYPE(QCameraInfo)

Camera::Camera() : ui(new Ui::Camera)
{
    ui->setupUi(this);

    m_VideoSurface.reset(new VideoSurface(ui->video, ui->multipartUrStatus));

    connect(ui->cameraComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectCamera(int)));
    connect(ui->newScanPushButton, SIGNAL(clicked()), this, SLOT(newScan()));
    connect(m_VideoSurface.data(), SIGNAL(newResult(const std::set<std::string, UrCompare>&)), this, SLOT(showResult(const std::set<std::string, UrCompare>&)));

    // init available cameras combo box
    for (const auto& camera : m_AvailableCameras)
    {
        ui->cameraComboBox->addItem(camera.description() + " - " + camera.deviceName());
    }

    // set default camera as the current camera
    for (int i = 0; i < m_AvailableCameras.size(); ++i)
    {
        if (m_AvailableCameras[i] == QCameraInfo::defaultCamera())
        {
            ui->cameraComboBox->setCurrentIndex(i);
        }
    }
}

void Camera::setCamera(const QCameraInfo &cameraInfo)
{
    if (!m_Camera.isNull())
    {
        m_Camera->stop();
        m_Camera->unload();
    }

    m_Camera.reset(new QCamera(cameraInfo));

    connect(m_Camera.data(), QOverload<QCamera::Error>::of(&QCamera::error), this, &Camera::displayCameraError);

    m_Camera->setViewfinder(m_VideoSurface.data());

    m_Camera->load();

    updateCaptureMode();
    const auto videoCapture = new QMediaRecorder(m_Camera.data());
    if (videoCapture->supportedResolutions().contains(PREFERRED_RESOLUTION))
    {
        QCameraViewfinderSettings settings;
        settings.setResolution(PREFERRED_RESOLUTION);
        m_Camera->setViewfinderSettings(settings);
    }

    m_Camera->start();
}

void Camera::selectCamera(const int i)
{
    setCamera(m_AvailableCameras[i]);
}

void Camera::updateCaptureMode()
{
    QCamera::CaptureModes captureMode = QCamera::CaptureVideo;
    if (m_Camera->isCaptureModeSupported(captureMode))
    {
        m_Camera->setCaptureMode(captureMode);
    }
}

void Camera::newScan()
{
    ui->stackedWidget->setCurrentIndex(0);
    m_VideoSurface->clear();
    m_VideoSurface->resumeScanning();
}

void Camera::showResult(const std::set<std::string, UrCompare>& results)
{
    m_VideoSurface->pauseScanning();

    for (const auto& result : results)
    {
        ui->resultTextBrowser->append(QString::fromStdString(result));
    }
    ui->resultTextBrowser->append("\n");

    ui->stackedWidget->setCurrentIndex(1);
}

void Camera::displayCameraError()
{
    QMessageBox::warning(this, tr("Camera Error"), m_Camera->errorString());
}
