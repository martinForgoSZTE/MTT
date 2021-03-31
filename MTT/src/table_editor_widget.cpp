#include "table_editor_widget.h"

#include <QPushButton>
#include <QDialogButtonBox>
#include <QSqlTableModel>
#include <QTableView>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlDatabase>
#include <QComboBox>


TableEditor::TableEditor(QSqlDatabase& db, const QString &tableName, QWidget *parent)
    : QWidget(parent)
{
    model = new QSqlTableModel(this, db);
    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    auto x = model->select();

    //model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    //model->setHeaderData(1, Qt::Horizontal, tr("First name"));
    //model->setHeaderData(2, Qt::Horizontal, tr("Last name"));

    view = new QTableView;
    view->setModel(model);
    view->resizeColumnsToContents();

    submitButton = new QPushButton(tr("Submit"));
    submitButton->setDefault(true);
    revertButton = new QPushButton(tr("&Revert"));
    quitButton = new QPushButton(tr("Quit"));

    buttonBox = new QDialogButtonBox(Qt::Vertical);
    buttonBox->addButton(submitButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(submitButton, &QPushButton::clicked, this, &TableEditor::submit);
    connect(revertButton, &QPushButton::clicked,  model, &QSqlTableModel::revertAll);
    connect(quitButton, &QPushButton::clicked, this, &TableEditor::close);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(view);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Database Cached Table"));
}

void TableEditor::submit()
{
    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
    } else {
        model->database().rollback();
        QMessageBox::warning(this, tr("Cached Table"),
                             tr("The database reported an error: %1")
                             .arg(model->lastError().text()));
    }
}

void TableEditor::SetComboBox(const QString &tableName)
{
    combo = new QComboBox;
    combo->addItems({tableName + "_Data", tableName + "_Meta"});
    this->layout()->addWidget(combo);
}




