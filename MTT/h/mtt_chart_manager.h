#pragma once

#include "mtt_basechart.h"
#include "mtt_piechart.h"
#include "mtt_Coordinate.h"
#include "db_entry.h"
#include <QVector>
#include <QMap>

#include <functional>

class QWidget;

QT_CHARTS_USE_NAMESPACE

class ChartFunctionalFactory
{
    QMap<CHART_TYPES, std::function<BaseChart*()>> m_factories;

public:
    ChartFunctionalFactory()
    {
        m_factories[CHART_TYPES::PIECHART] = [] {return new CustomPieChart(); };
    }
    BaseChart* createChart(CHART_TYPES type) { return m_factories[type](); }
};

class ChartsManager : public QObject
{
    Q_OBJECT

public:
    explicit ChartsManager(QWidget* parent);
    ~ChartsManager() = default;

    void setChart(CHART_TYPES type, const Custom_SQLite_Data_Wrapper& dataWrapper);
    void setData(const Custom_SQLite_Data_Wrapper& dataWrapper);
    BaseChart* GetChart() const
    {
        return m_pChart;
    }
    QVector<Coordinate> GetSelectedCoordinates() const
    {
        return m_selectedCoordinatesOnMap;
    }

signals:
    void gettingDataToCoordinates(QVector<Coordinate> coords);

public slots:
    void onClickedOntoMapPoint(const Coordinate& coord);

private:
    BaseChart* m_pChart;
    ChartFunctionalFactory m_factory;
    QWidget* m_pParent;

    QVector<Coordinate> m_selectedCoordinatesOnMap;
};
