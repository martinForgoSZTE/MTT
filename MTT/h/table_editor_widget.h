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


class TableEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TableEditor(QSqlDatabase& db,const QString &tableName, QWidget *parent = nullptr);
    void SetComboBox(const QString &tableName);

    QTableView* GetTableView() const
    {
        return view;
    }

private slots:
    void submit();

private:
    QPushButton *submitButton;
    QPushButton *revertButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QSqlTableModel *model;
    QTableView* view;
    QComboBox* combo;
    QStringList tables;
};












#endif
