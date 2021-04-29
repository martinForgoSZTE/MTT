#include <mapwidget.h>

#include <QImage>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "circle_graphics_item.h"

MapWidget::MapWidget(QWidget* parent) noexcept
    : QWidget(parent), m_pImage(new QImage), m_pScene(new QGraphicsScene()), m_pView( new QGraphicsView(this))
{
    QImage tmp;
    tmp.load(":/images/hungary.jpg");
    *m_pImage = tmp.scaled( 800, 1400, Qt::KeepAspectRatio);
    m_pScene->addPixmap(QPixmap::fromImage(*m_pImage));
    m_pScene->setSceneRect(m_pImage->rect());

/*    Coordinate coord;
    coord.map_coord.x = 28;
    coord.map_coord.y =  348;
    auto* item = new CircleGraphicsItem(coord);
    m_pScene->addItem(item);*/

    m_pView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_pView->setScene(m_pScene);
    m_pView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}


QGraphicsView* MapWidget::GetGraphicsView() const
{
    return m_pView;
}
