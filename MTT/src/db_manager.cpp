#include "db_manager.h"
#include "file_manager.h"
#include "db_entry.h"
#include "entry_position.h"
#include "constants.h"

#include <QSqlQuery>
#include <QFile>
#include <QVector>
#include <QSet>

// void CreateTableIfNotExists(const QString& driver, const QString& connName, const QString& tableName, const QString& dbPath, const QString&, ...); params into a blob obj

QVector<Coordinate> DB_Manager::GetCoordinates(const QString& tableName)
{
    QVector<Coordinate> mapped_coords;

    if(!m_CommonDB.isOpen())
        m_CommonDB.open();
    if(!m_StoreDB.isOpen())
        m_StoreDB.open();

    auto commonTables = GetRealTables(m_CommonDB);

    QString sqlAttach = QString("ATTACH DATABASE '%1' AS common").arg(QDir::toNativeSeparators(COMMON_DB_PATH));
    QSqlQuery q_attach = m_StoreDB.exec(sqlAttach);

    QSqlQuery query(m_StoreDB);
    bool isCountyRead = false;
    for(auto& commonTable : commonTables)
    {
        QString queryString;
        if(commonTable.contains(COUNTY_TABLE, Qt::CaseInsensitive) && !isCountyRead)
        {
            isCountyRead = true;
            queryString = "select " + tableName + ".area_name, al.longi, al.lati from " + tableName + ","
                    "(select County.name as CountyName, County_Seat.longitude as longi, County_Seat.latitude as lati from County, County_Seat where County.county_seat=County_Seat.name) AS al"
                    "where al.CountyName=" + tableName + ".area_name";
        }
        else
        {
            queryString = "SELECT " + tableName + ".area_name, " + commonTable + ".longitude, " + commonTable + ".latitude FROM " + tableName + ", " + commonTable + " WHERE " + tableName + ".area_name=" + commonTable + ".name";
        }

        q_attach.exec(queryString);
        qDebug() << q_attach.lastError().text();
        while(q_attach.next())
        {
           Coordinate coord;
           coord.area = q_attach.value(0).toString();
           coord.geo_coord.longitude = q_attach.value(1).toDouble();
           coord.geo_coord.latitude = q_attach.value(2).toDouble();
           mapped_coords.append(coord);
        }
    }

    return mapped_coords;
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
            for(const auto& entry : lregions)
            {
                query.exec("insert into LRegions(name, longitude, latitude) values('" + entry.name + "', " + entry.longitude + ", " + entry.latitude + ")");
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
            Record_Wrapper wrapper = man.parse(filename);
            tableName = wrapper.tableName;
            QString tableNameData = tableName + "_Data";
            QString tableNameMeta = tableName + "_Meta";
            if(!CheckIfTableExists(m_StoreDB, tableNameData) && !CheckIfTableExists(m_StoreDB, tableNameMeta))
            {

                QSqlQuery query(m_StoreDB);
                query.exec("create table if not exists " + tableNameData + "(area_name varchar(100), year varchar(10), data varchar(20))");
                query.exec("create table if not exists " + tableNameMeta + "(area_name varchar(100), capital integer, county integer, region integer, large_region integer)");
                for(auto* record : wrapper.records)
                {
                    if(record->entries.size())
                    {
                        query.prepare("insert into " + tableNameMeta + " values(?,?,?,?,?)");
                        query.addBindValue(record->entries[0]->area_name);
                        query.addBindValue(record->info.capital);
                        query.addBindValue(record->info.county);
                        query.addBindValue(record->info.region);
                        query.addBindValue(record->info.large_region);
                        query.exec();
                    }
                    for(auto* entry : record->entries)
                    {
                        query.exec("insert into " + tableNameData + " values('" + entry->area_name + "', '" + entry->year + "', '" + entry->data + "')");
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
