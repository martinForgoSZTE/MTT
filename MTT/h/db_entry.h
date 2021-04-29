#ifndef DB_ENTRY_H
#define DB_ENTRY_H

#include <QString>
#include <QVector>
#include <QStringList>


struct DB_Record
{
    struct area_info
    {
        int county = 0;
        int capital = 0;
        int region = 0;
        int large_region = 0;
    };
    struct DB_Entry
    {
        DB_Entry(const QString& area, const QString& year, const QString& data) : area_name{area}, year{year}, data{data}{}
        DB_Entry() = default;
        ~DB_Entry() = default;
        QString area_name;
        QString year;
        QString data;
    };

    area_info info;
    QVector<DB_Entry*> entries;
};

//TODO Data_Wrapper
struct Record_Wrapper
{
    QString horizontalText = "year";
    QString verticalText = "data";
    QString tableName;
    QVector<size_t> years;
    QString selectedYear;

    //"records" mean here all of the data in one year across the areas
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
