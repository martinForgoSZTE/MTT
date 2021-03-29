#ifndef TABLE_EDITOR_WIDGET_H
#define TABLE_EDITOR_WIDGET_H

#include <QWidget>

class QPushButton;
class QDialogButtonBox;
class QSqlTableModel;
class QTableView;


class TableEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TableEditor(const QString &tableName, QWidget *parent = nullptr);

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
};












#endif
