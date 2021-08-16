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

#include "multiparturstatus.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QPen>

namespace qurscanner
{

MultipartUrProgressBar::MultipartUrProgressBar(QWidget *parent)
    : QWidget(parent)
    , m_NumParts(1)
    , m_CompletedParts()
{
}

void MultipartUrProgressBar::Reset()
{
    m_NumParts = 1;
    m_CompletedParts.clear();
}

void MultipartUrProgressBar::SetNumParts(const size_t value)
{
    m_NumParts = value;
}

void MultipartUrProgressBar::SetCompletedParts(const std::set<size_t>& parts)
{
   m_CompletedParts = parts;
}

void MultipartUrProgressBar::paintEvent(QPaintEvent*)
{
    if (m_NumParts == 0)
    {
        return;
    }

    QPainter painter(this);

    QPen pen(Qt::green);
    pen.setWidth(1);
    painter.setPen(pen);

    const double widthPerPart = (width() - 1) / m_NumParts;
    for (size_t i = 0; i < m_NumParts; ++i)
    {
        if (m_CompletedParts.count(i) > 0)
        {
            painter.fillRect(QRectF(i * widthPerPart, 0, widthPerPart, height() - 1), Qt::green);
        }
        else
        {
            painter.drawRect(QRectF(i * widthPerPart, 0, widthPerPart, height() - 1));
        }
    }

    painter.end();
}

MultipartUrStatus::MultipartUrStatus(QWidget* parent)
    : QWidget(parent)
    , m_CancelButton("Cancel")
{
    QHBoxLayout* layout = new QHBoxLayout(this);

    layout->addWidget(&m_ProgressBar);
    layout->addWidget(&m_CancelButton);

    layout->setStretch(0, 1);

    connect(&m_CancelButton, SIGNAL(clicked(bool)), this, SIGNAL(Cancel()));
    connect(&m_CancelButton, SIGNAL(clicked(bool)), &m_ProgressBar, SLOT(Reset()));
}

void MultipartUrStatus::UpdateProgress(const size_t value, const std::set<size_t> &parts)
{
    m_ProgressBar.SetNumParts(value);
    m_ProgressBar.SetCompletedParts(parts);
}

} // namespace qurscanner
