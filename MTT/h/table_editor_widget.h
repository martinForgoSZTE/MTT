#ifndef TABLE_EDITOR_WIDGET_H
#define TABLE_EDITOR_WIDGET_H

#include <QWidget>
#include <QStringList>

class QPushButton;
class QDialogButtonBox;
class QSqlTableModel;
class QTableView;
class QSqlDatabase;
class QComboBox;
class MainWindow;
class QHBoxLayout;


class TableEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TableEditor(QWidget *parent = nullptr);
    void SetComboBox(const QString &tableName);
    void RemoveComboBox();
    void SetModelView(QSqlDatabase& db,const QString &tableName);
    void SetCustomLayout();
    QString getCurrentPureTableName() const;

private slots:
    void onSaveToDB();
    void onSerializeToDB();

public slots:
    void onCurrentTextChanged(const QString& table);

private:
    QPushButton *saveDBButton;
    QPushButton *serializeToDBButton;
    QPushButton *revertButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QSqlTableModel *model;
    QTableView* view;
    QComboBox* combo;
    QStringList tables;
    QHBoxLayout *mainLayout;

    MainWindow* parent;

    QString m_currentSelectedTablePureName;
};

#endif
