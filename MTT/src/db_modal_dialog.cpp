#include "db_modal_dialog.h"

#include <QPushButton>
#include <QComboBox>
#include <QStringList>
#include <QHBoxLayout>
#include <QVBoxLayout>


DBModal::DBModal(const QStringList& list, QDialog* parent)
    : QDialog(parent)
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);
    okButton = new QPushButton("Select table");
    cancelButton = new QPushButton("Back");
    comboBox = new QComboBox;
    comboBox->addItems(list);

    vlayout->addWidget(comboBox);
    vlayout->addWidget(okButton);
    vlayout->addWidget(cancelButton);
    
    setLayout(vlayout);
    this->setMinimumSize(300, 50);
    setModal(true);

    connect(okButton, &QPushButton::pressed, this, &DBModal::accept);
    connect(cancelButton, &QPushButton::pressed, this, &DBModal::reject);
    connect(okButton, &QPushButton::pressed, this, &DBModal::OnAccepted);
}


void DBModal::OnAccepted()
{
    SetSelectedTable(comboBox->itemText(comboBox->currentIndex()));
}


void DBModal::SetSelectedTable(const QString& szSelected)
{
    selectedTable = szSelected;
}

QString DBModal::GetSelectedTable() const
{
    return selectedTable;
}
