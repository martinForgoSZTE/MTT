#include <mapwidget.h>

#include <QImage>
#include <QGraphicsView>
#include <QGraphicsScene>


MapWidget::MapWidget(QWidget* parent) noexcept
    : QWidget(parent), m_pImage(new QImage), m_pScene(new QGraphicsScene()), m_pView( new QGraphicsView(this))
{
    QImage tmp;
    tmp.load(":/images/hungary.jpg");
    *m_pImage = tmp.scaled( 800, 1400, Qt::KeepAspectRatio);
    m_pScene->addPixmap(QPixmap::fromImage(*m_pImage));
    m_pScene->setSceneRect(m_pImage->rect());

    m_pView->setScene(m_pScene);
}


QGraphicsView* MapWidget::GetGraphicsView() const
{
    return m_pView;
}
