#include "mtt_chart_manager.h"
#include <algorithm>


ChartsManager::ChartsManager(QWidget* parent)
    : m_pChart{nullptr}, m_factory{}, m_pParent{parent}
{}

void ChartsManager::setChart(CHART_TYPES type, const Custom_SQLite_Data_Wrapper& dataWrapper)
{
    if(m_pChart)
        delete m_pChart;
    m_pChart = m_factory.createChart(type);
    m_pChart->setParent(m_pParent);
    m_pChart->SetChartData(dataWrapper);

    //a delete m_pChart miatt fentebb, nem kell disconnect az előzőre
    connect(m_pChart, &BaseChart::changedYear, this, &ChartsManager::onChangedYear);
}

void ChartsManager::setData(const Custom_SQLite_Data_Wrapper& dataWrapper)
{
    m_pChart->SetChartData(dataWrapper);
}

void ChartsManager::onClickedOntoMapPoint(const Coordinate& coord)
{
    if(std::find(m_selectedCoordinatesOnMap.begin(), m_selectedCoordinatesOnMap.end(), coord) == m_selectedCoordinatesOnMap.end())
    {
        m_selectedCoordinatesOnMap.push_back(coord);
    }
    else
    {
        m_selectedCoordinatesOnMap.removeOne(coord);
    }

    emit requestToSetChartData(m_selectedCoordinatesOnMap);
}

void ChartsManager::onChangedYear(const QString& newYear)
{
    emit requestToSetChartData(m_selectedCoordinatesOnMap, newYear);
}
