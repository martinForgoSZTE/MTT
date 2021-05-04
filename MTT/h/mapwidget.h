#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QVector>
#include "mtt_Coordinate.h"


class QImage;
class QGraphicsScene;
class QGraphicsView;

class MapWidget : public  QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget* parent = nullptr) noexcept;
    MapWidget(const MapWidget&) = delete;
    MapWidget& operator=(const MapWidget&) = delete;
    MapWidget(MapWidget&&) = delete;
    MapWidget& operator=(MapWidget&&) = delete;

    void AddItemsToScene(const QVector<Coordinate>& coords);
    QGraphicsView* GetGraphicsView() const;
    QGraphicsScene* GetGraphicsScene() const{return m_pScene;}

signals:
    void clickedOntoMapPoint(Coordinate coord);

private:
    QImage *m_pImage;
    QGraphicsScene* m_pScene;
    QGraphicsView* m_pView;
};


#endif
