#include "multiparturstatus.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QPen>

#include <iostream>

MultipartUrProgressBar::MultipartUrProgressBar(QWidget *parent)
    : QWidget(parent)
{
}

void MultipartUrProgressBar::Reset()
{
    m_NumParts = 0;
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
    connect(&m_CancelButton, SIGNAL(clicked(bool)), this, SLOT(hide()));
}

void MultipartUrStatus::SetNumParts(const size_t value)
{
    m_ProgressBar.SetNumParts(value);
}

void MultipartUrStatus::SetCompletedParts(const std::set<size_t>& parts)
{
    m_ProgressBar.SetCompletedParts(parts);
}

