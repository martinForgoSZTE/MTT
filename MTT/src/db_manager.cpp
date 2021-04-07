#include "db_manager.h"
#include "file_manager.h"
#include "db_entry.h"
#include "county.h"
#include "county_seat.h"
#include "region.h"
#include "large_region.h"

#include <QSqlQuery>
#include <QFile>
#include <QVector>
#include <tuple>
#include <type_traits>
#include <QSet>

static const QString COMMON_DB_PATH = QDir::toNativeSeparators("./common.db");
static const QString STORE_DB_PATH = QDir::toNativeSeparators("./store.db");
static const QString STORE_DB_PREFIX = "store";
inline const QString CONN_NAME = "StoreConn";

//TODO iterating through a tuple's elements -> 1 method instead of 3
// void CreateTableIfNotExists(const QString& driver, const QString& connName, const QString& tableName, const QString& dbPath, const QString&, ...); params into a blob obj
static const QString CREATE_COUNTY_TABLE_STRING = "create table if not exists County(name varchar(50), county_seat varchar(50))";
static const QString CREATE_COUNTY_SEAT_TABLE_STRING = "create table if not exists County_Seat("
                                                       "name varchar(50),"
                                                       "longitude real,"
                                                       "latitude real"
                                                       ")";


/*
template <class Tuple, class F, std::size_t... I>
F tuple_for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<I...>)
{
    (f(std::get<I>(tuple)), ...);
    return f;
}

template <class Tuple, class F>
constexpr decltype(auto) for_each(Tuple&& tuple, F&& f)
{
    return tuple_for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f),
                         std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}
*/


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
        CreateCountiesIfNotExists(driver);
        CreateCountySeatsIfNotExists(driver);
        CreateRegionsIfNotExists(driver);
        CreateLRegionsIfNotExists(driver);

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

    void DB_Manager::CreateCountiesIfNotExists(const QString& driver)
    {
        if(!m_CommonDB.isValid())
        {
            m_CommonDB = QSqlDatabase::addDatabase(driver, "CommonConn");
            m_CommonDB.setDatabaseName(COMMON_DB_PATH);
        }
        if(!m_CommonDB.isOpen())
        {
            if(!m_CommonDB.open())
            {
                QString e = m_CommonDB.lastError().driverText();
                qDebug(e.toUtf8());
                return;
            }
        }
        if(!CheckIfTableExists(m_CommonDB, "County"))
        {
            File_Manager man;
            QVector<County> counties =  man.GetCounties();

            QSqlQuery query(m_CommonDB);
            query.exec("create table if not exists Counties(name varchar(50), county_seat varchar(50))");
            for(auto& entry : counties)
            {
                query.exec("insert into County(name, county_seat) values('" + entry.name + "', '" + entry.county_seat + "')");
            }
        }
    }

    void DB_Manager::CreateCountySeatsIfNotExists(const QString& driver)
    {
        if(!m_CommonDB.isValid())
        {
            m_CommonDB = QSqlDatabase::addDatabase(driver, "CommonConn");
            m_CommonDB.setDatabaseName(COMMON_DB_PATH);
        }
        if(!m_CommonDB.isOpen())
        {
            if(!m_CommonDB.open())
            {
                QString e = m_CommonDB.lastError().driverText();
                qDebug(e.toUtf8());
                return;
            }
        }

        if(!CheckIfTableExists(m_CommonDB, "County_Seat"))
        {
            File_Manager man;
            QVector<County_Seat> county_seats =  man.GetCountySeats();

            QSqlQuery query(m_CommonDB);
            query.exec("create table if not exists County_Seats("
                       "name varchar(50),"
                       "longitude real,"
                       "latitude real"
                       ")");
            for(auto& entry : county_seats)
            {
                query.exec("insert into County_Seat(name, longitude, latitude) values('" + entry.name + "', " + entry.longitude + ", " + entry.latitude + ")");
            }
        }
    }

    void DB_Manager::CreateRegionsIfNotExists(const QString& driver)
    {
        if(!m_CommonDB.isValid())
        {
            m_CommonDB = QSqlDatabase::addDatabase(driver, "CommonConn");
            m_CommonDB.setDatabaseName(COMMON_DB_PATH);
        }
        if(!m_CommonDB.isOpen())
        {
            if(!m_CommonDB.open())
            {
                QString e = m_CommonDB.lastError().driverText();
                qDebug(e.toUtf8());
                return;
            }
        }

        if(!CheckIfTableExists(m_CommonDB, "Regions"))
        {
            File_Manager man;
            QVector<Region> regions =  man.GetRegions();

            QSqlQuery query(m_CommonDB);
            query.exec("create table if not exists Regions("
                       "name varchar(50),"
                       "longitude real,"
                       "latitude real"
                       ")");
            for(auto& entry : regions)
            {
                query.exec("insert into Regions(name, longitude, latitude) values('" + entry.name + "', " + entry.longitude + ", " + entry.latitude + ")");
            }
        }
    }


    void DB_Manager::CreateLRegionsIfNotExists(const QString& driver)
    {
        if(!m_CommonDB.isValid())
        {
            m_CommonDB = QSqlDatabase::addDatabase(driver, "CommonConn");
            m_CommonDB.setDatabaseName(COMMON_DB_PATH);
        }
        if(!m_CommonDB.isOpen())
        {
            if(!m_CommonDB.open())
            {
                QString e = m_CommonDB.lastError().driverText();
                qDebug(e.toUtf8());
                return;
            }
        }

        if(!CheckIfTableExists(m_CommonDB, "LRegions"))
        {
            File_Manager man;
            QVector<Large_Region> lregions =  man.GetLargeRegions();

            QSqlQuery query(m_CommonDB);
            query.exec("create table if not exists LRegions("
                       "name varchar(50),"
                       "longitude real,"
                       "latitude real"
                       ")");
            for(auto& entry : lregions)
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

     if(QSqlDatabase::connectionNames().contains(CONN_NAME))
         QSqlDatabase::removeDatabase(CONN_NAME);

     m_StoreDB = QSqlDatabase::addDatabase(szDriver, CONN_NAME);
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

 QSet<QString> DB_Manager::GetTables() const
 {
     QSet<QString> rSet;

     QSqlQuery query = m_StoreDB.exec("SELECT name FROM sqlite_master WHERE type='table'");
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

 QList<QString> DB_Manager::GetRealTables() const
 {
     QList<QString> rList;

     QSqlQuery query(m_StoreDB);
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

         auto tables = GetRealTables();
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
