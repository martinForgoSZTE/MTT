#ifndef DB_ENTRY_H
#define DB_ENTRY_H

#include <QString>
#include <QVector>
#include <memory>

struct DB_Record
{
    struct area_info
    {
        int county = 0;
        int capital = 0;
        int region = 0;
        int large_region = 0;
    };
    area_info info;

    struct DB_Entry
    {
        QString area_name;
        QString year;
        QString data;
    };
    QVector<DB_Record::DB_Entry*> entries;
};

struct Record_Wrapper
{
    QString horizontalText = "year";
    QString verticalText = "data";
    QString tableName;
    QVector<size_t> years;
    QVector<DB_Record*> records;

    void FillYears(QStringList& years)
    {
        for(auto& year : years)
        {
            this->years.push_back(year.toUInt());
        }
    }
};



#endif
