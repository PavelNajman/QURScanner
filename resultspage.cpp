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

#include "resultspage.h"

#include <QVBoxLayout>

#include <lifehash.hpp>

namespace qurscanner
{

ResultsPage::ResultsPage(QWidget* parent)
    : QWidget(parent)
    , m_ResultsTextBrowser(new QTextBrowser)
    , m_NewScanPushButton(new QPushButton("New scan"))
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_ResultsTextBrowser);
    layout->addWidget(m_NewScanPushButton);

    auto textOption = m_ResultsTextBrowser->document()->defaultTextOption();
    textOption.setAlignment(Qt::AlignHCenter);
    m_ResultsTextBrowser->document()->setDefaultTextOption(textOption);

    connect(m_NewScanPushButton, SIGNAL(clicked(bool)), this, SIGNAL(NewScan()));
}

void ResultsPage::AppendResults(const ur::UR& ur, const std::set<std::string, UrCompare>& results)
{
    const auto lifeHash = LifeHash::make_from_data(ur.cbor());

    QImage image(lifeHash.width, lifeHash.height, QImage::Format_RGB888);
    for (size_t r = 0; r < lifeHash.height; ++r)
    {
        for (size_t c = 0; c <lifeHash.width; ++c)
        {
            const auto i = 3 * (r * lifeHash.width + c);
            image.setPixelColor(c, r, QColor(lifeHash.colors[i], lifeHash.colors[i+1], lifeHash.colors[i+2]));
        }
    }

    m_LifeHashImageId++;
    m_ResultsTextBrowser->document()->addResource(QTextDocument::ImageResource, QUrl("lifehash://" + QString::number(m_LifeHashImageId)), QVariant(image.scaled(64, 64)));

    m_ResultsTextBrowser->append(QString("<img src=\"lifehash://") + QString::number(m_LifeHashImageId) + "\" />");
    for (const auto& result : results)
    {
        m_ResultsTextBrowser->append(QString::fromStdString(result));
    }
    m_ResultsTextBrowser->append("\n");
}

} // namespace qurscanner
