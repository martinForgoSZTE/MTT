#include "mtt_piechart.h"
#include "constants.h"

#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>

#include <QDebug>

QT_CHARTS_USE_NAMESPACE

PieChartWidget::PieChartWidget(QWidget *parent)
    : BaseChartWidget(parent), m_series(nullptr), m_slice(nullptr), m_chart(nullptr)
{
    m_themeComboBox = new QComboBox();
    m_themeComboBox->addItem("Qt", QChart::ChartThemeQt);
    m_themeComboBox->addItem("Light", QChart::ChartThemeLight);
    m_themeComboBox->addItem("Dark", QChart::ChartThemeDark);
    m_themeComboBox->addItem("High Contrast", QChart::ChartThemeHighContrast);

    m_legendCheckBox = new QCheckBox();

    m_pYearCombo = new QComboBox();
    m_pYearCombo->setPlaceholderText("Select a Year!");
    //it's an overloaded signal, so static_cast is needed with the help of a "pointer to member"
    connect(m_pYearCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &PieChartWidget::onComboYearChanged);

    QScrollArea *settingsScrollBar = new QScrollArea();
    QWidget *settingsContentWidget = new QWidget();

    QFormLayout *chartSettingsLayout = new QFormLayout(settingsContentWidget);
    chartSettingsLayout->addRow("Year", m_pYearCombo);
    chartSettingsLayout->addRow("Theme", m_themeComboBox);
    chartSettingsLayout->addRow("Legend", m_legendCheckBox);
    QGroupBox *chartSettings = new QGroupBox("Chart");
    chartSettings->setLayout(chartSettingsLayout);

    connect(m_themeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &PieChartWidget::updateChartSettings);
    connect(m_legendCheckBox, &QCheckBox::toggled, this, &PieChartWidget::updateChartSettings);

    // slice settings
    m_sliceName = new QLineEdit("<click on a slice>");
    m_sliceName->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_sliceName->setReadOnly(true);
    m_sliceValue = new QDoubleSpinBox();
    m_sliceValue->setReadOnly(true);
    m_sliceLabelVisible = new QCheckBox();
    m_sliceLabelArmFactor = new QDoubleSpinBox();
    m_sliceLabelArmFactor->setSingleStep(0.01);
    m_sliceExploded = new QCheckBox();
    m_sliceExplodedFactor = new QDoubleSpinBox();
    m_sliceExplodedFactor->setSingleStep(0.01);
    m_labelPosition = new QComboBox(this);
    m_labelPosition->addItem("Outside", QPieSlice::LabelOutside);
    m_labelPosition->addItem("Inside horizontal", QPieSlice::LabelInsideHorizontal);
    m_labelPosition->addItem("Inside tangential", QPieSlice::LabelInsideTangential);
    m_labelPosition->addItem("Inside normal", QPieSlice::LabelInsideNormal);

    QFormLayout *sliceSettingsLayout = new QFormLayout(settingsContentWidget);
    sliceSettingsLayout->addRow("Label", m_sliceName);
    sliceSettingsLayout->addRow("Value", m_sliceValue);
    sliceSettingsLayout->addRow("Label visible", m_sliceLabelVisible);
    sliceSettingsLayout->addRow("Label position", m_labelPosition);
    sliceSettingsLayout->addRow("Label arm length", m_sliceLabelArmFactor);
    sliceSettingsLayout->addRow("Exploded", m_sliceExploded);
    sliceSettingsLayout->addRow("Explode distance", m_sliceExplodedFactor);
    QGroupBox *sliceSettings = new QGroupBox("Selected slice");
    sliceSettings->setLayout(sliceSettingsLayout);

    connect(m_sliceLabelVisible, &QCheckBox::toggled, this, &PieChartWidget::updateSliceSettings);
    connect(m_sliceLabelVisible, &QCheckBox::toggled, this, &PieChartWidget::updateSliceSettings);
    connect(m_sliceLabelArmFactor,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &PieChartWidget::updateSliceSettings);
    connect(m_sliceExploded, &QCheckBox::toggled, this, &PieChartWidget::updateSliceSettings);
    connect(m_sliceExplodedFactor,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &PieChartWidget::updateSliceSettings);
    connect(m_labelPosition,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &PieChartWidget::updateSliceSettings);

    m_chartView = new QChartView();

    QVBoxLayout *settingsLayout = new QVBoxLayout();
    settingsLayout->addWidget(chartSettings);
    settingsLayout->addWidget(sliceSettings);

    settingsContentWidget->setLayout(settingsLayout);
    settingsScrollBar->setWidget(settingsContentWidget);
    settingsScrollBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    baseLayout = new QGridLayout();
    baseLayout->addWidget(settingsScrollBar, 0, 0);
    baseLayout->addWidget(m_chartView, 0, 1);
    setLayout(baseLayout);

    setWindowTheme(static_cast<QChart::ChartTheme>(m_themeComboBox->currentData().toInt()));
    updateChartSettings();
}


void PieChartWidget::updateChartSettings()
{
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(m_themeComboBox->itemData(m_themeComboBox->currentIndex()).toInt());
    m_chartView->chart()->setTheme(theme);
    setWindowTheme(theme);

    if (m_legendCheckBox->checkState() == Qt::Checked)
        m_chartView->chart()->legend()->show();
    else
        m_chartView->chart()->legend()->hide();
}

void PieChartWidget::updateSliceSettings()
{
    if (!m_slice)
        return;

    m_slice->setLabel(m_sliceName->text());

    m_slice->setValue(m_sliceValue->value());

    m_slice->setLabelVisible(m_sliceLabelVisible->isChecked());
    m_slice->setLabelArmLengthFactor(m_sliceLabelArmFactor->value());
    m_slice->setLabelPosition((QPieSlice::LabelPosition)m_labelPosition->currentIndex());

    m_slice->setExploded(m_sliceExploded->isChecked());
    m_slice->setExplodeDistanceFactor(m_sliceExplodedFactor->value());
}

void PieChartWidget::handleSliceClicked(QPieSlice *slice)
{
    m_slice = static_cast<CustomSlice*>(slice);

    // It is needed to block these objects' signals cus they may trigger some not expected behaviour (their signals, which are emitted, are also used in a slot)
    m_sliceName->blockSignals(true);
    m_sliceName->setText(slice->label());
    m_sliceName->blockSignals(false);

    // value changed signal
    m_sliceValue->blockSignals(true);
    m_sliceValue->setValue(slice->value());
    m_sliceValue->blockSignals(false);

    //toggled signal
    m_sliceLabelVisible->blockSignals(true);
    m_sliceLabelVisible->setChecked(slice->isLabelVisible());
    m_sliceLabelVisible->blockSignals(false);

    //value changed
    m_sliceLabelArmFactor->blockSignals(true);
    m_sliceLabelArmFactor->setValue(slice->labelArmLengthFactor());
    m_sliceLabelArmFactor->blockSignals(false);

    //value changed
    m_labelPosition->blockSignals(true);
    m_labelPosition->setCurrentIndex(slice->labelPosition());
    m_labelPosition->blockSignals(false);

    // toggled
    m_sliceExploded->blockSignals(true);
    m_sliceExploded->setChecked(slice->isExploded());
    m_sliceExploded->blockSignals(false);

    //value changed
    m_sliceExplodedFactor->blockSignals(true);
    m_sliceExplodedFactor->setValue(slice->explodeDistanceFactor());
    m_sliceExplodedFactor->blockSignals(false);
}

void PieChartWidget::onDataChanged()
{
    if(m_data.entries.size())
    {
        // create chart
        m_chart = new QChart;
        m_chart->setTitle("PieChart: " + m_data.tableName);
        m_chart->setAnimationOptions(QChart::AllAnimations);

        m_series = new QPieSeries();
        m_series->setLabelsVisible();
        connect(m_series, &QPieSeries::clicked, this, &PieChartWidget::handleSliceClicked);

        for(auto* entry : m_data.entries)
        {
            for(auto* record : entry->records)
            {
                auto* slice = new CustomSlice();
                slice->setLabel(record->area_name);
                double val = record->data.toDouble();
                slice->setValue(val);
                *m_series << slice;
            }
        }

        m_chart->addSeries(m_series);

        QChart* prev = nullptr;
        if(m_chartView->chart())
             prev = m_chartView->chart();

        m_chartView->setChart(m_chart);

        if(prev != nullptr)
            delete prev;

        fillYearsCombo();
        updateChartSettings();
    }
}

void PieChartWidget::fillYearsCombo()
{
    for(const auto& year : m_data.years)
    {
        m_pYearCombo->addItem(QString::number(year), QString::number(year));
    }
}

void PieChartWidget::onComboYearChanged(int index)
{
    if(index != -1)
        emit changedYear(m_pYearCombo->itemData(index).toString());
}


///////////////////////////////////////////////////////////////////////////////////////////////////


CustomSlice::CustomSlice(QObject *parent)
    : QPieSlice(parent)
{
    connect(this, &CustomSlice::hovered, this, &CustomSlice::showHighlight);
}

void CustomSlice::showHighlight(bool show)
{
    if (show) {
        QBrush brush = this->brush();
        m_originalBrush = brush;
        brush.setColor(brush.color().lighter());
        setBrush(brush);
    } else {
        setBrush(m_originalBrush);
    }
}
