#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <QFile>
#include <QVector>

#include "db_entry.h"
#include "entry_position.h"

class QTextStream;

class File_Manager
{
public:
    File_Manager() = default;
    ~File_Manager() = default;

    Custom_SQLite_Data_Wrapper parse(QString);
    QVector<EntryPosition> GetCounties();
    QVector<EntryPosition> GetCountySeats();
    QVector<EntryPosition> GetRegions();
    QVector<EntryPosition> GetLargeRegions();

private:
    bool CheckEncodingIsUTF8(QTextStream&) const;
    void CheckAreaInfo(typename DB_Entry::area_info&, QString&) const;

    QFile m_InputFile;

};



#endif
