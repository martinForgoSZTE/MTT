#pragma once

#include "mtt_basechart.h"
#include "mtt_piechart.h"
#include "mtt_barchart.h"
#include "mtt_Coordinate.h"
#include "db_entry.h"
#include <QVector>
#include <QMap>

#include <functional>

class QWidget;

QT_CHARTS_USE_NAMESPACE

class ChartFunctionalFactory
{
    QMap<CHART_TYPES, std::function<BaseChartWidget*()>> m_factories;

public:
    ChartFunctionalFactory()
    {
        m_factories[CHART_TYPES::PIECHART] = [] { return new PieChartWidget(); };
        m_factories[CHART_TYPES::BARCHART] = [] { return new BarChartWidget(); };
    }
    BaseChartWidget* createChart(CHART_TYPES type) { return m_factories[type](); }
};

class ChartsManager : public QObject
{
    Q_OBJECT

public:
    explicit ChartsManager(QWidget* parent);
    ~ChartsManager() = default;

    void setChart(CHART_TYPES type, const Custom_SQLite_Data_Wrapper& dataWrapper);
    void setData(const Custom_SQLite_Data_Wrapper& dataWrapper);
    BaseChartWidget* GetChart() const
    {
        return m_pChart;
    }
    QVector<Coordinate> GetSelectedCoordinates() const
    {
        return m_selectedCoordinatesOnMap;
    }

signals:
    void requestToSetChartData(QVector<Coordinate> coords, const QString& year = "");
    void requestToSetChartData(QVector<Coordinate> coords, const QString& startInterval, const QString& endInterval);

public slots:
    void onClickedOntoMapPoint(const Coordinate& coord);

private slots:
    void onChangedYear(const QString& newYear);
    void onChangedYear(const QString& startInterval, const QString& endInterval);

private:
    //egyszerre egy chart tartozik hozza jelen esetben
    BaseChartWidget* m_pChart;
    ChartFunctionalFactory m_factory;
    QWidget* m_pParent;

    QVector<Coordinate> m_selectedCoordinatesOnMap;
};
