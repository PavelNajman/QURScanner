#include "resultspage.h"

#include <QVBoxLayout>

#include <lifehash.hpp>

namespace urscanner
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

} // namespace urscanner
