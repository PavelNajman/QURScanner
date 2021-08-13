#include "urscanner.h"

#include <bc-ur.hpp>

namespace urscanner
{

UrScanner::UrScanner(QWidget* parent)
    : QStackedWidget(parent)
    , m_ScanPage(new ScanPage)
    , m_ResultsPage(new ResultsPage)
{
    this->addWidget(m_ScanPage);
    this->addWidget(m_ResultsPage);

    connect(m_ScanPage, SIGNAL(NewResult(const ur::UR&, std::set<std::string,UrCompare>)), m_ResultsPage, SLOT(AppendResults(const ur::UR&, std::set<std::string,UrCompare>)));
    connect(m_ScanPage, SIGNAL(NewResult(const ur::UR&, std::set<std::string,UrCompare>)), this, SLOT(ShowResultsPage()));
    connect(m_ResultsPage, SIGNAL(NewScan()), m_ScanPage, SLOT(NewScan()));
    connect(m_ResultsPage, SIGNAL(NewScan()), this, SLOT(ShowScanPage()));
}

void UrScanner::ShowResultsPage()
{
    setCurrentIndex(1);
}

void UrScanner::ShowScanPage()
{
    setCurrentIndex(0);
}

} // namespace urscanner
