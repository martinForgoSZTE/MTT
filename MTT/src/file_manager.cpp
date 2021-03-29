#include "file_manager.h"

#include <QTextStream>
#include <QTextCodec>
#include <QByteArray>

#include <string>

static const QString COUNTY_CSV_PATH = "../MTT/resources/counties.csv";
static const QString COUNTY_SEAT_CSV_PATH = "../MTT/resources/county_seats.csv";
static const QString REGION_CSV_PATH = "../MTT/resources/regions.csv";
static const QString LARGE_REGION_CSV_PATH = "../MTT/resources/large_regions.csv";
constexpr std::size_t DELIMITER_ROW = 29;


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
Record_Wrapper File_Manager::parse(QString fileName)
{
    Record_Wrapper wrapper;
    auto& records = wrapper.records;
    m_InputFile.setFileName(fileName );
    if(m_InputFile.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&m_InputFile);
        if(false == CheckEncodingIsUTF8(ts))
        {
            return wrapper;
        }

        ts.setCodec("UTF-8");
        std::size_t pos = 1;
        bool eof = false;

        while(!ts.atEnd() && !eof)
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
                    if(pos <= 32)   //TODO: nem jó explicit szám mert mi van ha csak régió
                    {
                        if(line.at(0) == '$')
                        {
                            pos--;
                            auto chunks = line.split(';');
                            auto data = chunks[0].split(',');
                            wrapper.verticalText = data[1];
                        }
                        else
                        {
                            DB_Record* record = new DB_Record;
                            auto fields = line.split(";");
                            std::size_t endYear = wrapper.years.size();
                            std::size_t startYear = wrapper.years[0];
                            std::size_t yearCounter = startYear;

                            CheckAreaInfo(record->info, fields[1]);
                            QString area_name = fields[0];

                            for(std::size_t i = 2; i < endYear; ++i)
                            {
                                DB_Record::DB_Entry* entry = new DB_Record::DB_Entry;
                                entry->area_name = area_name;
                                entry->year = QString::number(yearCounter);
                                yearCounter++;
                                entry->data = fields[i];
                                record->entries.push_back(entry);
                            }
                            records.push_back(record);
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

void File_Manager::CheckAreaInfo(typename DB_Record::area_info& info , QString& data) const
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


QVector<County> File_Manager::GetCounties()
{
    QFile counties(COUNTY_CSV_PATH);
    QVector<County> ret;
    if(counties.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&counties);
        ts.setCodec("UTF-8");
        while(!ts.atEnd())
        {
            QString line = ts.readLine();
            County entry;
            auto cols = line.split(";");
            entry.name = cols[0];
            entry.county_seat = cols[1];
            ret.push_back(entry);
        }
    }
    return ret;
}

QVector<County_Seat> File_Manager::GetCountySeats()
{
    QFile countySeasts(COUNTY_SEAT_CSV_PATH);
    QVector<County_Seat> ret;
    if(countySeasts.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&countySeasts);
        ts.setCodec("UTF-8");
        while(!ts.atEnd())
        {
            QString line = ts.readLine();
            County_Seat entry;
            auto cols = line.split(";");
            entry.name = cols[0];
            entry.latitude = cols[1];
            entry.longitude = cols[2];
            ret.push_back(entry);
        }
    }
    return ret;
}

QVector<Region> File_Manager::GetRegions()
{
    QFile regions(REGION_CSV_PATH);
    QVector<Region> ret;
    if(regions.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&regions);
        ts.setCodec("UTF-8");
        while(!ts.atEnd())
        {
            QString line = ts.readLine();
            Region entry;
            auto cols = line.split(";");
            entry.name = cols[0];
            entry.latitude = cols[1];
            entry.longitude = cols[2];
            ret.push_back(entry);
        }
    }
    return ret;
}

QVector<Large_Region> File_Manager::GetLargeRegions()
{
    QFile lregions(COUNTY_SEAT_CSV_PATH);
    QVector<Large_Region> ret;
    if(lregions.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&lregions);
        ts.setCodec("UTF-8");
        while(!ts.atEnd())
        {
            QString line = ts.readLine();
            Large_Region entry;
            auto cols = line.split(";");
            entry.name = cols[0];
            entry.latitude = cols[1];
            entry.longitude = cols[2];
            ret.push_back(entry);
        }
    }
    return ret;
}

