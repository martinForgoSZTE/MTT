#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <QFile>
#include <QVector>

#include "db_entry.h"
#include "county.h"
#include "county_seat.h"
#include "region.h"
#include "large_region.h"

class QTextStream;

class File_Manager
{
public:
    File_Manager() = default;
    ~File_Manager() = default;

    Record_Wrapper parse(QString);
    QVector<County> GetCounties();
    QVector<County_Seat> GetCountySeats();
    QVector<Region> GetRegions();
    QVector<Large_Region> GetLargeRegions();

private:
    bool CheckEncodingIsUTF8(QTextStream&) const;
    void CheckAreaInfo(typename DB_Record::area_info&, QString&) const;

    QFile m_InputFile;

};



#endif
