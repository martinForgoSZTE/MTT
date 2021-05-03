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

#include <algorithm>

QT_CHARTS_USE_NAMESPACE

BarChart::BarChart(QWidget *parent)
    : BaseChart(parent)
{
    m_pYearCombo = new QComboBox();
    m_pYearCombo->setPlaceholderText("Select a Year!");
    m_pStartInterval = new QComboBox();
    m_pStartInterval->setPlaceholderText("Select the start Year!");
    m_pEndInterval = new QComboBox();
    m_pEndInterval->setPlaceholderText("Select the end Year!");

    //it's an overloaded signal, so static_cast is needed with the help of a "pointer to member"
    connect(m_pYearCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &BarChart::onComboYearChanged);

    QScrollArea *settingsScrollBar = new QScrollArea();
    QWidget *settingsContentWidget = new QWidget();

    QFormLayout *chartSettingsLayout = new QFormLayout(settingsContentWidget);
    chartSettingsLayout->addRow("Year", m_pYearCombo);
    QGroupBox *chartSettings = new QGroupBox("Custom Year");
    chartSettings->setLayout(chartSettingsLayout);

    QVBoxLayout *settingsLayout = new QVBoxLayout();
    settingsLayout->addWidget(chartSettings);

    settingsContentWidget->setLayout(settingsLayout);
    settingsScrollBar->setWidget(settingsContentWidget);
    settingsScrollBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_chartView = new QChartView();
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setMinimumSize(640, 480);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(m_chartView, 1, 0);
    mainLayout->addWidget(settingsScrollBar, 1, 1);

    setLayout(mainLayout);
}

void BarChart::onDataChanged()
{
    if(!m_data.entries.size())
    {
        qDebug() << "Barchart: Stored data is empty!";
        return;
    }
    m_series = new QBarSeries;
    m_chart = new QChart;
    m_chart->setTitle("Barchart: " + m_data.tableName);
    m_chart->setAnimationOptions(QChart::AllAnimations);

    QStringList categories;
    for(auto* record : m_data.entries[0]->records)
    {
        QBarSet* set = new QBarSet(record->area_name);
        m_series->append(set);
    }


    auto barsets = m_series->barSets();
    auto findBarSet = [barsets](const QString& area) -> QBarSet*{
        for(auto* barset : barsets)
        {
            if(barset->label() == area)
                return barset;
        }
        return nullptr;
    };

    for(auto* entry : m_data.entries)
    {
        if(entry->records.size())
            categories << entry->records[0]->year;
        for(auto* record : entry->records)
        {
            auto* barset = findBarSet(record->area_name);
            if(barset != nullptr)
            {
                barset->append(record->data.toDouble());
            }
        }
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

void BarChart::fillYearsCombo()
{
    for(const auto& year : m_data.years)
    {
        m_pYearCombo->addItem(QString::number(year), QString::number(year));
    }
}

void BarChart::onComboYearChanged(int idx)
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
