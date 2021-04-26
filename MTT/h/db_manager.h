#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <QtSql>
#include <QString>
#include <QVariant>
#include <tuple>
#include <QStringList>
#include <vector>

#include "mtt_utility.h"
#include "mtt_Coordinate.h"
#include "constants.h"


class QSqlQuery;

class DB_Manager
{
public:
    DB_Manager(const QString& driver);
    ~DB_Manager() = default;

    struct DB_Blob
    {
        QString driver;
        QString connName;
        QString tableName;
        QString dbPath;
        QString createTableString;
        QString insertIntoString;
        QSqlDatabase& db;
        std::tuple<QVariant> values;
    };

    QString CreateCustomDB(const QString& filename);
    QSet<QString> GetTables(const QSqlDatabase&) const;
    void SetStoreDB(const QString& szPath = ":memory:", const QString& szDriver = SQLITE_DRIVER);
    QSqlDatabase& GetStoreDB();
    QList<QString> GetRealTables(const QSqlDatabase&) const;
    QVector<Coordinate> GetCoordinates(const QString& tableName);

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

    QSqlDatabase m_CommonDB;
    QSqlDatabase m_StoreDB;
};



#endif
