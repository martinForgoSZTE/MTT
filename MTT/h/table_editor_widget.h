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
class QHBoxLayout;
class QVBoxLayout;
class QFormLayout;
class QGroupBox;


class TableEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TableEditor(QWidget *parent = nullptr);
    void SetComboBox(const QString &tableName);
    void RemoveComboBox();
    void SetModelView(QSqlDatabase& db,const QString &tableName);
    QString getCurrentPureTableName() const;

signals:
    void serializeToDB();
    void currentTableChanged(const QString& table);

private slots:
    void onSaveToDB();

private:
    void SetCustomLayout();

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
    QVBoxLayout* vLayout;
    QWidget* settingsContentWidget;
    QGroupBox* comboSettings;
    QFormLayout* settingsLayout;

    QString m_currentSelectedTablePureName;
};

#endif
