#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <QtSql>
#include <QString>
#include <QVariant>
#include <tuple>

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

    void CreateCustomDB(const QString& driver, const QString& filename, const QString& tableName = "Test");
    void CreateCustomDB(const DB_Blob);

private:
    void CreateCountiesIfNotExists(const QString&);
    void CreateCountySeatsIfNotExists(const QString&);
    void CreateRegionsIfNotExists(const QString&);
    void CreateLRegionsIfNotExists(const QString&);
    bool CheckIfTableExists(QSqlDatabase&, const QString& tableName);
    int GetSizeOfResultSet(QSqlDatabase&, QSqlQuery&);

    QSqlDatabase m_StoreDB;
    QSqlDatabase m_CommonDB;
};



#endif
