#pragma once

#include "mtt_basechart.h"

#include <QtCharts/QChartGlobal>
#include <QBarSet>

class QSqlTableModel;
class QComboBox;
class QCheckBox;
class QDoubleSpinBox;
class QLineEdit;

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
class QBarSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class BarChartWidget : public BaseChartWidget
{
    Q_OBJECT

public:
    explicit BarChartWidget(QWidget *parent = 0);
    ~BarChartWidget() = default;

private:
    void onDataChanged() override;
    void fillYearsCombo();

private slots:
    void onComboYearChanged(int);
    void onComboStartIntervalChanged(int);
    void onComboEndIntervalChanged(int);
    void updateChartSettings();
    void handleBarClicked(int index, QBarSet* barset);

private:
    QChart* m_chart;
    QSqlTableModel* m_model;
    QComboBox* m_pYearCombo;
    QComboBox* m_pStartInterval;
    QComboBox* m_pEndInterval;
    QComboBox* m_themeComboBox;
    QCheckBox* m_legendCheckBox;
    QLineEdit* m_BarSetIndexVal;
    QLineEdit* m_BarSetName;

    QBarSeries* m_series;
    QChartView* m_chartView;
};

