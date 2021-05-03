#pragma once

#include "db_entry.h"
#include <QWidget>
#include <QVector>

enum CHART_TYPES : int
{
    UNKNOWN = -1,
    PIECHART = 0,
    BARCHART = 1,
};


class BaseChart : public QWidget
{
    Q_OBJECT
public:
    explicit BaseChart(QWidget* parent = nullptr) : QWidget(parent){}
    virtual ~BaseChart() = default;

    void SetChartData(const Custom_SQLite_Data_Wrapper& data)
    {
        m_data = data;
        onDataChanged();
    }

signals:
    void changedYear(const QString& newYear);

protected:
    Custom_SQLite_Data_Wrapper m_data;

private:
    virtual void onDataChanged() = 0;
};
