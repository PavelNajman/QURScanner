#ifndef URSCANNER_H
#define URSCANNER_H

#include <QStackedWidget>

#include "scanpage.h"
#include "resultspage.h"

namespace urscanner
{

class UrScanner : public QStackedWidget
{
    Q_OBJECT
public:
    explicit UrScanner(QWidget* parent = nullptr);

private slots:
    void ShowResultsPage();
    void ShowScanPage();

private:
    ScanPage* m_ScanPage;
    ResultsPage* m_ResultsPage;
};

} // namespace urscanner

#endif // URSCANNER_H
