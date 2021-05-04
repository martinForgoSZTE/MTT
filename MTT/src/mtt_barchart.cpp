#include "mtt_barchart.h"


#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTableView>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QVXYModelMapper>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QVBarModelMapper>
#include <QtWidgets/QHeaderView>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

#include <QSqlTableModel>
#include <QComboBox>
#include <QScrollArea>
#include <QFormLayout>
#include <QGroupBox>
#include <QList>
#include <QSet>
#include <QVector>
#include <QDebug>
#include <QCheckBox>
#include <QLineEdit>
#include <QDoubleSpinBox>

#include <algorithm>

QT_CHARTS_USE_NAMESPACE

BarChartWidget::BarChartWidget(QWidget *parent)
    : BaseChartWidget(parent)
{
    m_pYearCombo = new QComboBox();
    m_pYearCombo->setPlaceholderText("Select a Year!");
    m_pStartInterval = new QComboBox();
    m_pStartInterval->setPlaceholderText("Select the start Year!");
    m_pEndInterval = new QComboBox();
    m_pEndInterval->setPlaceholderText("Select the end Year!");

    //it's an overloaded signal, so static_cast is needed with the help of a "pointer to member"
    connect(m_pYearCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &BarChartWidget::onComboYearChanged);
    connect(m_pStartInterval, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &BarChartWidget::onComboStartIntervalChanged);
    connect(m_pEndInterval, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &BarChartWidget::onComboEndIntervalChanged);

    QScrollArea *settingsScrollBar = new QScrollArea();
    QWidget *settingsContentWidget = new QWidget();

    QFormLayout *yearSettingsLayout = new QFormLayout(settingsContentWidget);
    yearSettingsLayout->addRow("Year", m_pYearCombo);
    QGroupBox *yearSettings = new QGroupBox("Custom Year");
    yearSettings->setLayout(yearSettingsLayout);

    ///////////////////////////////////////////////

    QFormLayout *yearIntervalSettingsLayout = new QFormLayout(settingsContentWidget);
    yearIntervalSettingsLayout->addRow("Start Year: ", m_pStartInterval);
    yearIntervalSettingsLayout->addRow("End Year: ", m_pEndInterval);
    QGroupBox *yearIntervalSettings = new QGroupBox("Custom Interval");
    yearIntervalSettings->setLayout(yearIntervalSettingsLayout);

    ///////////////////////////////////////////////

    m_legendCheckBox = new QCheckBox();
    m_themeComboBox = new QComboBox();
    m_themeComboBox->addItem("Qt", QChart::ChartThemeQt);
    m_themeComboBox->addItem("Light", QChart::ChartThemeLight);
    m_themeComboBox->addItem("Dark", QChart::ChartThemeDark);
    m_themeComboBox->addItem("High Contrast", QChart::ChartThemeHighContrast);
    m_BarSetName = new QLineEdit("<Click on a Bar>");
    m_BarSetName->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_BarSetName->setReadOnly(true);
    m_BarSetIndexVal = new QDoubleSpinBox();
    m_BarSetIndexVal->setReadOnly(true);

    connect(m_themeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &BarChartWidget::updateChartSettings);
    connect(m_legendCheckBox, &QCheckBox::toggled, this, &BarChartWidget::updateChartSettings);

    QFormLayout* chartSettingsLayout = new QFormLayout(settingsContentWidget);
    chartSettingsLayout->addRow("Theme: ", m_themeComboBox);
    chartSettingsLayout->addRow("Legend: ", m_legendCheckBox);
    chartSettingsLayout->addRow("Barset name: ", m_BarSetName);
    chartSettingsLayout->addRow("Bar value: ", m_BarSetIndexVal);
    QGroupBox* chartSettings = new QGroupBox("Chart");
    chartSettings->setLayout(chartSettingsLayout);

    ///////////////////////////////////////////////

    QVBoxLayout *settingsLayout = new QVBoxLayout();
    settingsLayout->addWidget(chartSettings);
    settingsLayout->addWidget(yearSettings);
    settingsLayout->addWidget(yearIntervalSettings);

    ///////////////////////////////////////////////

    settingsContentWidget->setLayout(settingsLayout);
    settingsScrollBar->setWidget(settingsContentWidget);
    settingsScrollBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    ///////////////////////////////////////////////

    m_chartView = new QChartView();
    m_chartView->setMinimumSize(640, 480);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(m_chartView, 1, 0);
    mainLayout->addWidget(settingsScrollBar, 1, 1);

    ///////////////////////////////////////////////

    setLayout(mainLayout);
    updateChartSettings();
    setWindowTheme(static_cast<QChart::ChartTheme>(m_themeComboBox->currentData().toInt()));
}

void BarChartWidget::updateChartSettings()
{
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(m_themeComboBox->currentData().toInt());
    m_chartView->chart()->setTheme(theme);
    setWindowTheme(theme);

    if (m_legendCheckBox->checkState() == Qt::Checked)
        m_chartView->chart()->legend()->show();
    else
        m_chartView->chart()->legend()->hide();
}

void BarChartWidget::onDataChanged()
{
    if(!m_data.entries.size())
    {
        qDebug() << "Barchart: Stored data is empty!";
        return;
    }
    m_series = new QBarSeries;
    connect(m_series, &QBarSeries::clicked, this, &BarChartWidget::handleBarClicked);
    m_chart = new QChart;
    m_chart->setTitle("Barchart: " + m_data.tableName);
    m_chart->setAnimationOptions(QChart::AllAnimations);

    //creating categories (years on X axis)
    QStringList categories;
    for(auto* record : m_data.entries[0]->records)
    {
        categories << record->year;
    }

    //creating barsets (areas belong to years as bars)
    for(auto* entry : m_data.entries)
    {
        if(entry->records.size())
        {
            QBarSet* set = new QBarSet(entry->records[0]->area_name);
            m_series->append(set);
        }
    }


    //findig corresponding barset
    auto barsets = m_series->barSets();
    auto findBarSet = [barsets](const QString& area) -> QBarSet*{
        for(auto* barset : barsets)
        {
            if(barset->label() == area)
                return barset;
        }
        return nullptr;
    };

    //filling barsets with values
    for(auto* entry : m_data.entries)
    {
        QBarSet* barset;
        QList<qreal> barsetValues;
        if(entry->records.size())
        {
           barset = findBarSet(entry->records[0]->area_name);
        }
        for(auto* record : entry->records)
        {
           barsetValues.append(record->data.toDouble());
        }
        barset->append(barsetValues);
    }

    m_chart->addSeries(m_series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    m_chart->addAxis(axisX, Qt::AlignBottom);
    m_series->attachAxis(axisX);
    QValueAxis *axisY = new QValueAxis();
    m_chart->addAxis(axisY, Qt::AlignLeft);
    m_series->attachAxis(axisY);

    QChart* prev = nullptr;
    if(m_chartView->chart())
         prev = m_chartView->chart();

    m_chartView->setChart(m_chart);

    if(prev != nullptr)
        delete prev;

    fillYearsCombo();
}

void BarChartWidget::fillYearsCombo()
{
    m_pYearCombo->clear();
    m_pStartInterval->clear();
    m_pEndInterval->clear();
    for(const auto& year : m_data.years)
    {
        m_pYearCombo->addItem(QString::number(year), QString::number(year));
        m_pStartInterval->addItem(QString::number(year), QString::number(year));
    }
}

void BarChartWidget::onComboYearChanged(int idx)
{
    if(idx != -1)
    {
        m_pStartInterval->blockSignals(true);
        m_pStartInterval->setCurrentIndex(-1);
        m_pStartInterval->blockSignals(false);
        m_pEndInterval->blockSignals(true);
        m_pEndInterval->setCurrentIndex(-1);
        m_pEndInterval->blockSignals(false);
        emit changedYear(m_pYearCombo->itemData(idx).toString());
    }
}

void BarChartWidget::onComboStartIntervalChanged(int)
{
    m_pYearCombo->blockSignals(true);
    m_pYearCombo->setCurrentIndex(-1);
    m_pYearCombo->blockSignals(false);
    for(const auto& year : m_data.years)
    {
        if(year >= m_pStartInterval->currentData().toDouble())
            m_pEndInterval->addItem(QString::number(year), QString::number(year));
    }
}

void BarChartWidget::onComboEndIntervalChanged(int)
{
    if(m_pStartInterval->currentIndex() == -1)
    {
        m_pEndInterval->setCurrentIndex(-1);
        return;
    }

    emit changedYear(m_pStartInterval->currentData().toString(), m_pEndInterval->currentData().toString());
}

void BarChartWidget::handleBarClicked(int index, QBarSet* barset)
{
    m_BarSetName->setText(barset->label());
    m_BarSetIndexVal->setValue(barset->at(index));
}

