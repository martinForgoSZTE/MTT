#include "mtt_charts.h"


ChartsManager::ChartsManager(QWidget* parent)
    : m_pChart{nullptr}, m_factory{}, m_pParent{parent}
{}

void ChartsManager::setChart(CHART_TYPES type, const QVector<Coordinate>& coords)
{
    if(m_pChart)
        delete m_pChart;
    m_pChart = m_factory.createChart(type);
    m_pChart->setParent(m_pParent);
    m_pChart->SetChartData(coords);
}

void ChartsManager::setData(const QVector<Coordinate>& coords)
{
    m_pChart->SetChartData(coords);
}
