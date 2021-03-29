#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>


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

    QGraphicsView* GetGraphicsView() const;

private:
    QImage *m_pImage;
    QGraphicsScene* m_pScene;
    QGraphicsView* m_pView;
};


#endif
