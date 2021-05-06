#pragma once

#include "db_entry.h"
#include <QWidget>
#include <QVector>
#include <QtCharts/QChartGlobal>
#include <QChart>



enum CHART_TYPES : int
{
    UNKNOWN = -1,
    PIECHART = 0,
    BARCHART = 1,
};

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class BaseChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseChartWidget(QWidget* parent = nullptr);
    virtual ~BaseChartWidget() = default;

    void SetChartData(const Custom_SQLite_Data_Wrapper& data);

signals:
    void changedYear(const QString& newYear);
    void changedYear(const QString& startInterval, const QString& endInterval);

protected:
    Custom_SQLite_Data_Wrapper m_data;

    virtual void onDataChanged() = 0;
    void setWindowTheme(QChart::ChartTheme);
};
