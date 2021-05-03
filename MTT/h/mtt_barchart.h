#pragma once

#include "mtt_basechart.h"

#include <QtCharts/QChartGlobal>

class QSqlTableModel;
class QComboBox;

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
class QBarSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class BarChart : public BaseChart
{
    Q_OBJECT

public:
    explicit BarChart(QWidget *parent = 0);
    ~BarChart() = default;

private:
    void onDataChanged() override;
    void fillYearsCombo();

private slots:
    void onComboYearChanged(int);
    void onComboStartIntervalChanged(int);
    void onComboEndIntervalChanged(int);

private:
    QChart* m_chart;
    QSqlTableModel* m_model;
    QComboBox* m_pYearCombo;
    QComboBox* m_pStartInterval;
    QComboBox* m_pEndInterval;

    QBarSeries* m_series;
    QChartView* m_chartView;
};
