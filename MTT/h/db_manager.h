#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <QtSql>
#include <QString>
#include <QVariant>
#include <tuple>
#include <QStringList>


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
    QSet<QString> GetTables() const;
    void SetStoreDB(const QString& szPath = ":memory:", const QString& szDriver = "QSQLITE");
    QSqlDatabase& GetStoreDB();
    QList<QString> GetRealTables() const;

    bool SerializeDB();

    bool storeIsInMemory;

private:
    void CreateCountiesIfNotExists(const QString&);
    void CreateCountySeatsIfNotExists(const QString&);
    void CreateRegionsIfNotExists(const QString&);
    void CreateLRegionsIfNotExists(const QString&);
    bool CheckIfTableExists(QSqlDatabase&, const QString& tableName);
    int GetSizeOfResultSet(QSqlDatabase&, QSqlQuery&);

    QSqlDatabase m_CommonDB;
    QSqlDatabase m_StoreDB;
};



#endif
