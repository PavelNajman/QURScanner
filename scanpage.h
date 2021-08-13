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

namespace urscanner
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

} // namespace urscanner

#endif // SCANPAGE_H
