#pragma once

#include "mtt_Coordinate.h"
#include <QWidget>

enum CHART_TYPES : int
{
    UNKNOWN = -1,
    PIECHART = 0,
};


class BaseChart
{
public:
    explicit BaseChart() = default;
    virtual ~BaseChart() = default;

    void SetChartData(const QVector<Coordinate> data)
    {
        m_data  = data;
    }
    virtual QWidget* get() const = 0;
    virtual void setParent(QWidget* widget) = 0;

private:
    QVector<Coordinate> m_data;
};
