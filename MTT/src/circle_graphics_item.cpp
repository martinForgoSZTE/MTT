#include "circle_graphics_item.h"
#include "constants.h"

#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QToolTip>

static qreal g_width = CIRCLE_ITEM_WIDTH;
static qreal g_height = CIRCLE_ITEM_HEIGHT;

CircleGraphicsItem::CircleGraphicsItem(const Coordinate& coord, QGraphicsItem *parent) : QGraphicsItem(parent), m_coord(coord)
{
    Pressed = false;
    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF CircleGraphicsItem::boundingRect() const
{
    return QRectF(qreal(m_coord.map_coord.x),
                  qreal(m_coord.map_coord.y),
                  g_width, g_height);
}

void CircleGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();


    if(Pressed)
    {
        QPen pen(Qt::red, 3);
        painter->setPen(pen);
    }
    else
    {
        QPen pen(m_coord.color, 3);
        painter->setPen(pen);
    }
    painter->drawEllipse(rect);
}

void CircleGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(Pressed)
    {
        Pressed = false;
    }
    else
    {
        Pressed = true;
    }
    emit clickedOntoMapPoint(m_coord);
    update();
    QGraphicsItem::mousePressEvent(event);
}

void CircleGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    setToolTip("Area: " + m_coord.area + "\n(Latitude; Longitude): (" + QString::number(m_coord.geo_coord.latitude) + ";" + QString::number(m_coord.geo_coord.longitude) + ")");
    QGraphicsItem::hoverMoveEvent(event);
}
