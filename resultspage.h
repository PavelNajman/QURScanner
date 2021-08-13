#ifndef RESULTSPAGE_H
#define RESULTSPAGE_H

#include <set>

#include <QWidget>
#include <QTextBrowser>
#include <QPushButton>

#include <bc-ur.hpp>

#include "urutils.h"

namespace urscanner
{

class ResultsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ResultsPage(QWidget* parent = nullptr);

public slots:
    void AppendResults(const ur::UR& ur, const std::set<std::string, UrCompare>& results);

signals:
    void NewScan();

private:
    QTextBrowser* m_ResultsTextBrowser;
    QPushButton* m_NewScanPushButton;

    size_t m_LifeHashImageId = 0;
};

} // namespace urscanner

#endif // RESULTSPAGE_H
