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

#include "qurscanner.h"

#include <bc-ur.hpp>

namespace qurscanner
{

QURScanner::QURScanner(QWidget* parent)
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

void QURScanner::ShowResultsPage()
{
    setCurrentIndex(1);
}

void QURScanner::ShowScanPage()
{
    setCurrentIndex(0);
}

} // namespace qurscanner
