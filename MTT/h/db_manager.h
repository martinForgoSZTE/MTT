#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <QtSql>
#include <QString>
#include <QVariant>
#include <QStringList>
#include <QVector>

#include "mtt_utility.h"
#include "mtt_Coordinate.h"
#include "constants.h"
#include "db_entry.h"


class QSqlQuery;

class DB_Manager
{
public:
    DB_Manager(const QString& driver);
    ~DB_Manager() = default;

    /*struct DB_Blob
    {
        QString driver;
        QString connName;
        QString tableName;
        QString dbPath;
        QString createTableString;
        QString insertIntoString;
        QSqlDatabase& db;
        std::tuple<QVariant> values; //binding values?
    };*/

    QString CreateCustomDB(const QString& filename);
    QSet<QString> GetTables(const QSqlDatabase&) const;
    void SetStoreDB(const QString& szPath = ":memory:", const QString& szDriver = SQLITE_DRIVER);
    QSqlDatabase& GetStoreDB();
    QList<QString> GetRealTables(const QSqlDatabase&) const;

    QVector<Coordinate> GetMappedCoordinates(const QString& tableName);
    //TODO area név szerint a kiszedett rekordokat selectálhatom a kiválasztott koordináta pontok alapján
    Custom_SQLite_Data_Wrapper GetRecords(const QString& tablename, const QVector<Coordinate>& coordinatesToSearchInDB, const QString& year = "");

    bool SerializeDB();

    bool storeIsInMemory;

private:
    void CreateCountiesIfNotExists();
    void CreateCountySeatsIfNotExists();
    void CreateRegionsIfNotExists();
    void CreateLRegionsIfNotExists();

    bool CheckIfTableExists(QSqlDatabase&, const QString& tableName);
    bool OpenDB(QSqlDatabase& db, const QString& conn, const QString& path);
    int GetSizeOfResultSet(QSqlDatabase&, QSqlQuery&);

    QVector<Coordinate> GetGeoCoordinatesFromTable(const QString& tableName);
    void MapEndPositions(QVector<Coordinate>& coords);
    void MapGeoCoordinates(QVector<Coordinate>& coords);

    QStringList GetYears(const QString& tablename);
    QString GetConcatenatedAreaSetToSearchInDB(const QVector<Coordinate>& coords);


    QSqlDatabase m_CommonDB;
    QSqlDatabase m_StoreDB;
};



#endif
