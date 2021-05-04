#ifndef DB_ENTRY_H
#define DB_ENTRY_H

#include <QString>
#include <QVector>
#include <QStringList>


struct DB_Entry
{
    struct area_info
    {
        int county = 0;
        int capital = 0;
        int region = 0;
        int large_region = 0;
    };
    struct DB_Record
    {
        DB_Record(const QString& area, const QString& year, const QString& data) : area_name{area}, year{year}, data{data}{}
        DB_Record() = default;
        ~DB_Record() = default;
        QString area_name;
        QString year;
        QString data;
    };

    area_info info;
    QVector<DB_Record*> records;
};

struct Custom_SQLite_Data_Wrapper
{
    QString horizontalText = "year";
    QString verticalText = "data";
    QString tableName;
    QVector<size_t> years;

    //"entries" mean here all of the records in one area across the years
    QVector<DB_Entry*> entries;

    void FillYears(QStringList& years)
    {
        for(auto& year : years)
        {
            this->years.push_back(year.toUInt());
        }
    }
};



#endif
