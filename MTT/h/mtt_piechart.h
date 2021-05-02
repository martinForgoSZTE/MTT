#pragma once

#include "mtt_basechart.h"

#include <QtWidgets/QWidget>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartGlobal>


class QLineEdit;
class QPushButton;
class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QGridLayout;
class QScrollArea;

class CustomSlice;

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QPieSeries;
class QPieSlice;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class CustomPieChart : public BaseChart
{
    Q_OBJECT

public:
    explicit CustomPieChart(QWidget *parent = nullptr);

private slots:
    void updateChartSettings();
    void updateSliceSettings();
    void handleSliceClicked(QPieSlice *slice);
    void onComboYearChanged(int index);


private:
    void onDataChanged() override;
    void fillYearsCombo();

    QDoubleSpinBox *m_hPosition;
    QDoubleSpinBox *m_vPosition;
    QDoubleSpinBox *m_sizeFactor;
    QDoubleSpinBox *m_startAngle;
    QDoubleSpinBox *m_endAngle;
    QDoubleSpinBox *m_holeSize;

    QLineEdit *m_sliceName;
    QDoubleSpinBox *m_sliceValue;
    QCheckBox *m_sliceLabelVisible;
    QDoubleSpinBox *m_sliceLabelArmFactor;
    QCheckBox *m_sliceExploded;
    QDoubleSpinBox *m_sliceExplodedFactor;
    QComboBox *m_labelPosition;

    QComboBox *m_themeComboBox;
    QCheckBox *m_legendCheckBox;

    QChartView *m_chartView;
    QPieSeries *m_series;
    CustomSlice *m_slice;

    QGridLayout* baseLayout;
    QScrollArea *settingsScrollBar;
    QWidget *settingsContentWidget;

    QComboBox* m_pYearCombo;
    QChart *m_chart;
};


class CustomSlice : public QPieSlice
{
    Q_OBJECT

public:
    explicit CustomSlice(QObject *parent = nullptr);


private slots:
    void showHighlight(bool show);

private:
    QBrush m_originalBrush;
};
