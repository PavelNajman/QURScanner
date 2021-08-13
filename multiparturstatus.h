#ifndef MULTIPARTURSTATUS_H
#define MULTIPARTURSTATUS_H

#include <set>

#include <QWidget>
#include <QPushButton>

namespace urscanner
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

} // namespace urscanner

#endif // MULTIPARTURSTATUS_H
