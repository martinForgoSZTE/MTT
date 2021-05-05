#include "file_manager.h"
#include "constants.h"

#include <QTextStream>
#include <QTextCodec>
#include <QByteArray>
#include <QDir>

#include <string>
#include <algorithm>

#include "entry_position.h"


bool File_Manager::CheckEncodingIsUTF8(QTextStream& ts) const
{
    bool ret = false;
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QByteArray data = ts.readAll().toLocal8Bit();
    const QString text = codec->toUnicode(data.constData(), data.size(), &state);
    if (state.invalidChars > 0)
    {
        // Not a UTF-8 text
        ret = false;
        qDebug("Input file has an invalid UTF-8 encoding\n");
    }
    else
    {
        ret = true;
    }
    ts.seek(0); //set back to 0. position

    return ret;
}

//TODO: std::thread az egész parse
Custom_SQLite_Data_Wrapper File_Manager::parse(QString fileName)
{
    Custom_SQLite_Data_Wrapper wrapper;
    auto& entries = wrapper.entries;
    auto simplyfyChunks = [](QStringList& chunks) -> void { std::transform(chunks.begin(), chunks.end(), chunks.begin(),
        [](QString& chunk) { return chunk.simplified(); });
    };
    m_InputFile.setFileName(fileName );
    if(m_InputFile.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&m_InputFile);
        if(false == CheckEncodingIsUTF8(ts))
        {
            qDebug() << "Input file encoding is not UTF-8";
            return wrapper;
        }

        ts.setCodec("UTF-8");
        std::size_t pos = 1;
        bool eof = false;

        while(!eof)
        {
            QString line = ts.readLine();
            switch(pos)
            {
                case 1:
                {
                    auto chunks = line.split(';');
                    wrapper.tableName = chunks[0];
                    pos++;
                    break;
                }
                case 2:
                {
                    auto chunks = line.split(";;");
                    simplyfyChunks(chunks);
                    auto years = chunks[1].split(';');
                    wrapper.FillYears(years);
                    pos++;
                    break;
                }
                case 3:
                {
                    pos++;
                    break;
                }
                default:
                {
                    if(line != "END")
                    {
                        if(line.size() > 0)
                        {
                            DB_Entry* entry = new DB_Entry;
                            auto fields = line.split(";");
                            simplyfyChunks(fields);
                            std::size_t endYear = wrapper.years.size();
                            std::size_t startYear = wrapper.years[0];
                            std::size_t yearCounter = startYear;

                            CheckAreaInfo(entry->info, fields[1]);
                            QString area_name = fields[0];

                            for(std::size_t i = 2; i < endYear; ++i)
                            {
                                DB_Entry::DB_Record* record = new DB_Entry::DB_Record;
                                record->area_name = area_name;
                                record->year = QString::number(yearCounter);
                                yearCounter++;
                                record->data = fields[i];
                                record->data = record->data.replace(",", ".");
                                entry->records.push_back(record);
                            }
                            entries.push_back(entry);
                        }
                        pos++;
                    }
                    else
                    {
                        eof = true;
                    }
                }
            }
        }
    }
    return wrapper;
}

void File_Manager::CheckAreaInfo(typename DB_Entry::area_info& info , QString& data) const
{
    auto chunks = data.split(',');
    for(auto& chunk : chunks)
    {
        if(0 == strcmp("főváros", chunk.toLower().trimmed().toLocal8Bit().data()))
        {
            info.capital = 1;
        }
        else if(0 == strcmp("megye", chunk.toLower().trimmed().toLocal8Bit().data()))
        {
            info.county = 1;
        }
        else if(0 == strcmp("régió", chunk.toLower().trimmed().toLocal8Bit().data()))
        {
            info.region = 1;
        }
        else if(0 == strcmp("nagyrégió", chunk.toLower().trimmed().toLocal8Bit().data()))
        {
            info.large_region = 1;
        }
    }
}


QVector<EntryPosition> File_Manager::GetCounties()
{
    QFile counties(COUNTY_CSV_PATH);
    QVector<EntryPosition> ret;
    if(counties.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&counties);
        ts.setCodec("UTF-8");
        while(!ts.atEnd())
        {
            QString line = ts.readLine();
            EntryPosition entry;
            auto cols = line.split(";");
            entry.name = cols[0];
            entry.county_seat = cols[1];
            ret.push_back(entry);
        }
    }
    return ret;
}

QVector<EntryPosition> File_Manager::GetCountySeats()
{
    QFile countySeasts(COUNTY_SEAT_CSV_PATH);
    QVector<EntryPosition> ret;
    if(countySeasts.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&countySeasts);
        ts.setCodec("UTF-8");
        while(!ts.atEnd())
        {
            QString line = ts.readLine();
            EntryPosition entry;
            auto cols = line.split(";");
            entry.name = cols[0];
            entry.county_seat = cols[1];
            entry.longitude = cols[2];
            entry.latitude = cols[3];
            ret.push_back(entry);
        }
    }
    return ret;
}

QVector<EntryPosition> File_Manager::GetRegions()
{
    QFile regions(REGION_CSV_PATH);
    QVector<EntryPosition> ret;
    if(regions.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&regions);
        ts.setCodec("UTF-8");
        while(!ts.atEnd())
        {
            QString line = ts.readLine();
            EntryPosition entry;
            auto cols = line.split(";");
            entry.name = cols[0];
            entry.latitude = cols[1];
            entry.longitude = cols[2];
            ret.push_back(entry);
        }
    }
    return ret;
}

QVector<EntryPosition> File_Manager::GetLargeRegions()
{
    QFile lregions(LARGE_REGION_CSV_PATH);
    QVector<EntryPosition> ret;
    if(lregions.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&lregions);
        ts.setCodec("UTF-8");
        while(!ts.atEnd())
        {
            QString line = ts.readLine();
            EntryPosition entry;
            auto cols = line.split(";");
            entry.name = cols[0];
            entry.latitude = cols[1];
            entry.longitude = cols[2];
            ret.push_back(entry);
        }
    }
    return ret;
}

