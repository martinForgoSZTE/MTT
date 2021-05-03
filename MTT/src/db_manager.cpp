#include "db_manager.h"
#include "file_manager.h"
#include "db_entry.h"
#include "entry_position.h"
#include "constants.h"

#include <QSqlQuery>
#include <QFile>
#include <QVector>
#include <QSet>

#include <utility>


QVector<Coordinate> DB_Manager::GetMappedCoordinates(const QString& tableName)
{
    auto coords = GetGeoCoordinatesFromTable(tableName);
    MapEndPositions(coords);
    MapGeoCoordinates(coords);
    return coords;
}

void DB_Manager::MapEndPositions(QVector<Coordinate>& coords)
{
    Coordinate ref1;
    Coordinate ref2;

    ref1.geo_coord.latitude = 48.05242;
    ref1.geo_coord.longitude = 22.86507;
    ref1.map_coord.x = 763;
    ref1.map_coord.y = 158;

    ref2.geo_coord.latitude = 46.87530;
    ref2.geo_coord.longitude = 16.1541327;
    ref2.map_coord.x = 28;
    ref2.map_coord.y = 348;

    coords.push_back(ref1);
    coords.push_back(ref2);
}

void DB_Manager::MapGeoCoordinates(QVector<Coordinate>& coords)
{
    auto ref1 = coords.takeAt(coords.size()-1);
    auto ref2 = coords.takeAt(coords.size()-1);

    auto ref1_pair = std::make_pair(ref1.geo_coord, ref1.map_coord);
    auto ref2_pair = std::make_pair(ref2.geo_coord, ref2.map_coord);

    for(auto& coord : coords)
    {
        coord.map_coord = utilities::CalculateMapPosition(coord.geo_coord, ref1_pair, ref2_pair);
    }
}

QVector<Coordinate> DB_Manager::GetGeoCoordinatesFromTable(const QString& tableName)
{
    QVector<Coordinate> coords;

    if(!m_CommonDB.isOpen())
        m_CommonDB.open();
    if(!m_StoreDB.isOpen())
        m_StoreDB.open();

    auto commonTables = GetRealTables(m_CommonDB);

    QString sqlAttach = QString("ATTACH DATABASE '%1' AS common").arg(QDir::toNativeSeparators(COMMON_DB_PATH));
    QSqlQuery q_attach = m_StoreDB.exec(sqlAttach);

    for(const auto& commonTable : commonTables)
    {
        QString queryString =
                "SELECT " + tableName + ".area_name, " + commonTable + ".longitude, " + commonTable + ".latitude FROM " +
                tableName + ", " + commonTable + " WHERE " + tableName + ".area_name=" + commonTable + ".name";

        q_attach.exec(queryString);
        qDebug() << q_attach.lastError().text();
        while(q_attach.next())
        {
           Coordinate coord;
           coord.area = q_attach.value(0).toString();
           coord.geo_coord.longitude = q_attach.value(1).toDouble();
           coord.geo_coord.latitude = q_attach.value(2).toDouble();
           coords.append(coord);
        }
    }

    return coords;
}


DB_Manager::DB_Manager(const QString& driver)
{
    storeIsInMemory = true;
    if(!QSqlDatabase::isDriverAvailable(driver))
    {
        qDebug("ERROR: driver is not available...");
    }
    else
    {
        if(!QFile::exists(STORE_DB_PATH))
        {
            QFile file(STORE_DB_PATH);
            file.open(QIODevice::ReadWrite);
            file.close();
        }
        CreateCountiesIfNotExists();
        CreateCountySeatsIfNotExists();
        CreateRegionsIfNotExists();
        CreateLRegionsIfNotExists();

        if(m_CommonDB.isValid() && m_CommonDB.isOpen())
            m_CommonDB.close();
    }
}

    bool DB_Manager::CheckIfTableExists(QSqlDatabase& db, const QString& tableName)
    {
        QSqlQuery query(db);
        query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='" + tableName + "'");
        return GetSizeOfResultSet(db, query) > 0;
    }

    int DB_Manager::GetSizeOfResultSet(QSqlDatabase& db, QSqlQuery& query)
    {
        int numRows;
        if (db.driver()->hasFeature(QSqlDriver::QuerySize)) {
            numRows = query.size();
        } else {
            query.last();
            numRows = query.at() + 1;
            query.seek(-1);
        }
        return numRows;
    }

    bool DB_Manager::OpenDB(QSqlDatabase& db, const QString& conn, const QString& path)
    {
        if(!db.isValid())
        {
            db = QSqlDatabase::addDatabase(SQLITE_DRIVER, conn);
            db.setDatabaseName(path);
        }
        if(!db.isOpen())
        {
            if(!db.open())
            {
                QString e = db.lastError().driverText();
                qDebug(e.toUtf8());
                return false;
            }
        }
        return true;
    }

    void DB_Manager::CreateCountiesIfNotExists()
    {
        if(!OpenDB(m_CommonDB, COMMON_CONN_NAME, COMMON_DB_PATH))
        {
            return;
        }

        if(!CheckIfTableExists(m_CommonDB, COUNTY_TABLE))
        {
            File_Manager man;
            auto county_seats =  man.GetCounties();

            QSqlQuery query(m_CommonDB);
            query.exec(CREATE_TABLE_COUNTY);
            for(const auto& entry : county_seats)
            {
                query.exec("insert into County values('" + entry.name + "', '" + entry.county_seat + ")");
            }
        }
    }

    void DB_Manager::CreateCountySeatsIfNotExists()
    {
        if(!OpenDB(m_CommonDB, COMMON_CONN_NAME, COMMON_DB_PATH))
        {
            return;
        }

        if(!CheckIfTableExists(m_CommonDB, COUNTY_SEAT_TABLE))
        {
            File_Manager man;
            auto county_seats =  man.GetCountySeats();

            QSqlQuery query(m_CommonDB);
            query.exec(CREATE_TABLE_COUNTY_SEAT);
            for(const auto& entry : county_seats)
            {
                query.exec("insert into County_Seat(name, seat_name, longitude, latitude) values('" + entry.name + "', '" + entry.county_seat + "', " + entry.longitude + ", " + entry.latitude + ")");
            }
        }
    }

    void DB_Manager::CreateRegionsIfNotExists()
    {
        if(!OpenDB(m_CommonDB, COMMON_CONN_NAME, COMMON_DB_PATH))
        {
            return;
        }

        if(!CheckIfTableExists(m_CommonDB, REGIONS_TABLE))
        {
            File_Manager man;
            auto regions =  man.GetRegions();

            QSqlQuery query(m_CommonDB);
            query.exec(CREATE_TABLE_REGIONS);
            for(const auto& entry : regions)
            {
                query.exec("insert into Regions(name, longitude, latitude) values('" + entry.name + "', " + entry.longitude + ", " + entry.latitude + ")");
            }
        }
    }


    void DB_Manager::CreateLRegionsIfNotExists()
    {
        if(!OpenDB(m_CommonDB, COMMON_CONN_NAME, COMMON_DB_PATH))
        {
            return;
        }

        if(!CheckIfTableExists(m_CommonDB, LREGIONS_TABLE))
        {
            File_Manager man;
            auto lregions =  man.GetLargeRegions();

            QSqlQuery query(m_CommonDB);
            query.exec(CREATE_TABLE_LREGIONS);
            bool x = false;
            for(const auto& entry : lregions)
            {
                query.prepare("insert into LRegions(name, longitude, latitude) values(?,?,?)");
                query.addBindValue(entry.name);
                query.addBindValue(entry.longitude);
                query.addBindValue(entry.latitude);
                x = query.exec();
            }
        }
    }



    QString DB_Manager::CreateCustomDB(const QString& filename)
    {
        QString tableName;
        if(!m_StoreDB.isOpen())
        {
            if(!m_StoreDB.open())
            {
                QString e = m_StoreDB.lastError().driverText();
                qDebug(e.toUtf8());
                return tableName;
            }
        }

            File_Manager man;
            Custom_SQLite_Data_Wrapper wrapper = man.parse(filename);
            tableName = wrapper.tableName;
            QString tableNameData = tableName + "_Data";
            QString tableNameMeta = tableName + "_Meta";
            if(!CheckIfTableExists(m_StoreDB, tableNameData) && !CheckIfTableExists(m_StoreDB, tableNameMeta))
            {

                QSqlQuery query(m_StoreDB);
                query.exec("create table if not exists " + tableNameData + "(area_name varchar(100), year varchar(10), data varchar(20))");
                query.exec("create table if not exists " + tableNameMeta + "(area_name varchar(100), capital integer, county integer, region integer, large_region integer)");
                for(auto* entry : wrapper.entries)
                {
                    if(entry->records.size())
                    {
                        query.prepare("insert into " + tableNameMeta + " values(?,?,?,?,?)");
                        query.addBindValue(entry->records[0]->area_name);
                        query.addBindValue(entry->info.capital);
                        query.addBindValue(entry->info.county);
                        query.addBindValue(entry->info.region);
                        query.addBindValue(entry->info.large_region);
                        query.exec();
                    }
                    for(auto* record : entry->records)
                    {
                        query.exec("insert into " + tableNameData + " values('" + record->area_name + "', '" + record->year + "', '" + record->data + "')");
                    }
                }
            }
            return tableName;
    }

 void DB_Manager::SetStoreDB(const QString& szPath, const QString& szDriver)
 {
     if(szPath != ":memory:")
        storeIsInMemory = false;
     else
         storeIsInMemory = true;

     if(m_StoreDB.isValid() && m_StoreDB.isOpen())
     {
        m_StoreDB.close();
     }

     if(QSqlDatabase::connectionNames().contains(STORE_CONN_NAME))
         QSqlDatabase::removeDatabase(STORE_CONN_NAME);

     m_StoreDB = QSqlDatabase::addDatabase(szDriver, STORE_CONN_NAME);
     m_StoreDB.setDatabaseName(szPath);
     if(!m_StoreDB.open())
     {
         QString e = m_StoreDB.lastError().driverText();
         qDebug(e.toUtf8());
     }
 }

 QSqlDatabase& DB_Manager::GetStoreDB()
 {
    return m_StoreDB;
 }

 QSet<QString> DB_Manager::GetTables(const QSqlDatabase& db) const
 {
     QSet<QString> rSet;
     qDebug() << QDir::currentPath();
     QSqlQuery query = db.exec("SELECT name FROM sqlite_master WHERE type='table';");
     if(query.lastError().text() != "")
     {
        qDebug() << query.lastError().text();
     }

     while(query.next())
     {
         rSet.insert(query.value(0).toString().split("_")[0]);
     }

     return rSet;
 }

 QList<QString> DB_Manager::GetRealTables(const QSqlDatabase& db) const
 {
     QList<QString> rList;

     QSqlQuery query(db);
     query.exec("SELECT name FROM sqlite_master WHERE type='table'");
     while(query.next())
     {
         rList.append(query.value(0).toString());
     }

     return rList;
 }

 // only in memory tables are serialized
 bool DB_Manager::SerializeDB()
 {
     //TODO enum
     bool rbSucc = true;
     if(storeIsInMemory)
     {
         QString sqlAttach = QString("ATTACH DATABASE '%1' AS " + STORE_DB_PREFIX).arg(QDir::toNativeSeparators(STORE_DB_PATH));
         QSqlQuery q_attach = m_StoreDB.exec(sqlAttach);

         auto tables = GetRealTables(m_StoreDB);
         for(auto& table : tables)
         {
            if(CheckIfTableExists(m_StoreDB, STORE_DB_PREFIX + "." + table))
            {
                rbSucc = false;
                break;
            }
            QSqlQuery q_create_fdb(m_StoreDB);
            auto bSucc = q_create_fdb.exec("CREATE TABLE if not exists store." + table + " AS SELECT * from " + table );
            qDebug() << "Creating " + table + " in " + STORE_DB_PATH + ": " << q_create_fdb.lastError();
            if(!bSucc)
            {
                rbSucc = false;
            }
         }
     }
     else
     {
         rbSucc = false;
     }
     SetStoreDB(STORE_DB_PATH);
     return rbSucc;
 }

QStringList DB_Manager::GetYears(const QString& tablename)
{
    QStringList years;
    QSqlQuery query(m_StoreDB);
    query.prepare("SELECT year FROM " + tablename + " GROUP BY year order by year");
    if(!query.exec())
        qDebug() << query.lastError();
    while(query.next())
    {
        years.push_back(query.value(0).toString());
    }

    return years;
}

Custom_SQLite_Data_Wrapper DB_Manager::GetRecords(const QString& tablename, const QVector<Coordinate>& coordinatesToSearchInDB, const QString& startInterval, const QString& endInterval)
{
    QStringList years = GetYears(tablename);
    QString startYear = (startInterval == "" ? (years.isEmpty() ? "" : years[0]) : startInterval);
    QString endYear = (endInterval == "" ? (years.isEmpty() ? "" : years[0]) : endInterval);
    if(startYear == "" || endYear == "")
    {
        qDebug() << "Selected year is NULL!";
        return {};
    }
    QString area_list = GetConcatenatedAreaSetToSearchInDB(coordinatesToSearchInDB);

    Custom_SQLite_Data_Wrapper wrapper;
    wrapper.tableName = tablename.split("_")[0];
    wrapper.FillYears(years);

    QSqlQuery query(m_StoreDB);
    query.prepare("SELECT area_name, year, data FROM " + tablename + " WHERE area_name IN (" + area_list + ") AND year BETWEEN " + startYear + " AND " + endYear + " order by area_name, year");
    if(!query.exec())
        qDebug() << query.lastError();

    DB_Entry* prevEntry;
    QString prevArea = "";
    while(query.next())
    {
        DB_Entry::DB_Record* record = new DB_Entry::DB_Record(query.value(0).toString(), query.value(1).toString(), query.value(2).toString());
        if(query.value(0).toString() == prevArea)
        {
            prevEntry->records.append(record);
        }
        else
        {
            DB_Entry* newEntry = new DB_Entry;
            newEntry->records.append(record);
            wrapper.entries.push_back(newEntry);
            prevEntry = newEntry;
            prevArea = query.value(0).toString();
        }
    }

    return wrapper;
}

QString DB_Manager::GetConcatenatedAreaSetToSearchInDB(const QVector<Coordinate>& coords)
{
    QString ret = "";
    for(const auto& coord : coords)
    {
        if(coord.area != "")
            ret += ("'" + coord.area + "', ");
    }
    if(ret != "")
    {
        //removing last space and ','
        ret.chop(2);
    }
    return ret;
}
