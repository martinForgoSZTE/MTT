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

CustomPieChart::CustomPieChart(QWidget *parent)
    : BaseChart(parent), m_series(nullptr), m_slice(nullptr), m_chart(nullptr)
{
    m_themeComboBox = new QComboBox();
    m_themeComboBox->addItem("Qt", QChart::ChartThemeQt);
    m_themeComboBox->addItem("Light", QChart::ChartThemeLight);
    m_themeComboBox->addItem("Dark", QChart::ChartThemeDark);
    m_themeComboBox->addItem("High Contrast", QChart::ChartThemeHighContrast);

    m_legendCheckBox = new QCheckBox();

    m_pYearCombo = new QComboBox();
    m_pYearCombo->setPlaceholderText("Select a Year!");
    /*auto line_edit = m_pYearCombo->lineEdit();
    QPalette palette = line_edit->palette();
    palette.setColor(QPalette::PlaceholderText, Qt::red);
    palette.setColor(QPalette::Text, Qt::black);
    line_edit->setPalette(palette);*/
    //m_pYearCombo->addItem("PieChart", CHART_TYPES::PIECHART);


    QScrollArea *settingsScrollBar = new QScrollArea();
    QWidget *settingsContentWidget = new QWidget();

    QFormLayout *chartSettingsLayout = new QFormLayout(settingsContentWidget);
    chartSettingsLayout->addRow("Year", m_pYearCombo);
    chartSettingsLayout->addRow("Theme", m_themeComboBox);
    chartSettingsLayout->addRow("Legend", m_legendCheckBox);
    QGroupBox *chartSettings = new QGroupBox("Chart");
    chartSettings->setLayout(chartSettingsLayout);

    connect(m_themeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &CustomPieChart::updateChartSettings);
    connect(m_legendCheckBox, &QCheckBox::toggled, this, &CustomPieChart::updateChartSettings);

    // slice settings
    m_sliceName = new QLineEdit("<click on a slice>");
    m_sliceName->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_sliceValue = new QDoubleSpinBox();
    m_sliceValue->setMaximum(1000);
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

    connect(m_sliceName, &QLineEdit::textChanged, this, &CustomPieChart::updateSliceSettings);
    connect(m_sliceValue,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &CustomPieChart::updateSliceSettings);
    connect(m_sliceLabelVisible, &QCheckBox::toggled, this, &CustomPieChart::updateSliceSettings);
    connect(m_sliceLabelVisible, &QCheckBox::toggled, this, &CustomPieChart::updateSliceSettings);
    connect(m_sliceLabelArmFactor,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &CustomPieChart::updateSliceSettings);
    connect(m_sliceExploded, &QCheckBox::toggled, this, &CustomPieChart::updateSliceSettings);
    connect(m_sliceExplodedFactor,
            static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &CustomPieChart::updateSliceSettings);
    connect(m_labelPosition,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &CustomPieChart::updateSliceSettings);

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

    updateChartSettings();
}


void CustomPieChart::updateChartSettings()
{
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(m_themeComboBox->itemData(m_themeComboBox->currentIndex()).toInt());
    m_chartView->chart()->setTheme(theme);

    if (m_legendCheckBox->checkState() == Qt::Checked)
        m_chartView->chart()->legend()->show();
    else
        m_chartView->chart()->legend()->hide();
}

void CustomPieChart::updateSliceSettings()
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

void CustomPieChart::handleSliceClicked(QPieSlice *slice)
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

void CustomPieChart::onDataChanged()
{
    if(m_data.entries.size())
    {
        // create chart
        m_chart = new QChart;
        m_chart->setTitle("PieChart: " + m_data.tableName);
        m_chart->setAnimationOptions(QChart::AllAnimations);

        m_series = new QPieSeries();
        m_series->setLabelsVisible();
        connect(m_series, &QPieSeries::clicked, this, &CustomPieChart::handleSliceClicked);

        for(auto* record : m_data.entries[0]->records)
        {
            auto* slice = new CustomSlice();
            slice->setLabel(record->area_name);
            double val = record->data.toDouble();
            slice->setValue(val);
            *m_series << slice;
        }

        m_chart->addSeries(m_series);

        QChart* prev = nullptr;
        if(m_chartView->chart())
             prev = m_chartView->chart();

        m_chartView->setChart(m_chart);

        if(prev != nullptr)
            delete prev;

        //updateChartSettings();
    }
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
