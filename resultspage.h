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

#ifndef RESULTSPAGE_H
#define RESULTSPAGE_H

#include <set>

#include <QWidget>
#include <QTextBrowser>
#include <QPushButton>

#include <bc-ur.hpp>

#include "urutils.h"

namespace qurscanner
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

} // namespace qurscanner

#endif // RESULTSPAGE_H
