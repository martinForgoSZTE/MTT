#ifndef DB_MODAL_DIALOG_H
#define DB_MODAL_DIALOG_H

#include <QDialog>


class QPushButton;
class QComboBox;
class QStringList;

class DBModal : public QDialog
{
    Q_OBJECT;

public:
    explicit DBModal(const QStringList& list, QDialog* parent = nullptr);

private slots:
    void OnAccepted();

public:
    QString GetSelectedTable() const;


private:
    void SetSelectedTable(const QString&);

    QPushButton* okButton;
    QPushButton* cancelButton;
    QComboBox* comboBox;

    QString selectedTable;
};

#endif
