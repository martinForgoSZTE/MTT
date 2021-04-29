#pragma once

#include <QPainter>
#include <QGraphicsItem>
#include "mtt_Coordinate.h"

class CircleGraphicsItem :public QGraphicsItem
{
public:
    CircleGraphicsItem(const Coordinate& coord, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget) override;


    bool Pressed;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    Coordinate m_coord;
};
