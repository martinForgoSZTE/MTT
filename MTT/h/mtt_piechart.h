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

class CustomSlice;

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QPieSeries;
class QPieSlice;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class CustomPieChart : public QWidget, public BaseChart
{
    Q_OBJECT

public:
    explicit CustomPieChart(QWidget *parent = 0);

    void setParent(QWidget *parent) override{ setParent(parent); }
    CustomPieChart* get() const override{ return const_cast<CustomPieChart*>(this); };
public Q_SLOTS:
    void updateChartSettings();
    void CustomPieChart::updateSliceSettings();
    void handleSliceClicked(QPieSlice *slice);

private:
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

    QComboBox* m_pYearCombo;
};



class CustomSlice : public QPieSlice
{
    Q_OBJECT

public:
    CustomSlice(QString label, qreal value);


public Q_SLOTS:
    void showHighlight(bool show);

private:
    QBrush m_originalBrush;
};
