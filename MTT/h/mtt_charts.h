#pragma once

#include "mtt_basechart.h"
#include "mtt_piechart.h"
#include "mtt_Coordinate.h"
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

class ChartsManager
{

public:
    explicit ChartsManager(QWidget* parent);
    ~ChartsManager() = default;

    void setChart(CHART_TYPES type, const QVector<Coordinate>& coords);
    void setData(const QVector<Coordinate>& coords);
    void setParent(QWidget* parent){m_pParent = parent;}

    BaseChart* GetChart() const { return m_pChart; }

private:
    BaseChart* m_pChart;
    ChartFunctionalFactory m_factory;
    QWidget* m_pParent;
};
