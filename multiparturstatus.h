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

#ifndef MULTIPARTURSTATUS_H
#define MULTIPARTURSTATUS_H

#include <set>

#include <QWidget>
#include <QPushButton>

namespace qurscanner
{

class MultipartUrProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit MultipartUrProgressBar(QWidget* parent = nullptr);

    void SetNumParts(const size_t value);
    void SetCompletedParts(const std::set<size_t>& parts);

public slots:
    void Reset();

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    size_t m_NumParts;
    std::set<size_t> m_CompletedParts;
};

class MultipartUrStatus : public QWidget
{
    Q_OBJECT
public:
    explicit MultipartUrStatus(QWidget* parent = nullptr);

public slots:
    void UpdateProgress(const size_t value, const std::set<size_t>& parts);

signals:
    void Cancel();

private:
    MultipartUrProgressBar m_ProgressBar;
    QPushButton m_CancelButton;
};

} // namespace qurscanner

#endif // MULTIPARTURSTATUS_H
