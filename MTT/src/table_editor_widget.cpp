#include "table_editor_widget.h"
#include "mainwindow.h"

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
#include <QDir>
#include <QSqlDatabase>


TableEditor::TableEditor(QWidget *parent)
    : QWidget(parent),
      saveDBButton(nullptr),
      serializeToDBButton(nullptr),
      revertButton(nullptr),
      quitButton(nullptr),
      buttonBox(nullptr),
      model(nullptr),
      view(nullptr),
      combo(nullptr),
      mainLayout(nullptr),
      parent(qobject_cast<MainWindow*>(parent))
{
    saveDBButton = new QPushButton(tr("Submit"));
    saveDBButton->setDefault(true);
    revertButton = new QPushButton(tr("&Revert"));
    serializeToDBButton = new QPushButton(tr("Serialize"));

    buttonBox = new QDialogButtonBox(Qt::Vertical);
    buttonBox->addButton(saveDBButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(serializeToDBButton, QDialogButtonBox::ActionRole);

    connect(saveDBButton, &QPushButton::clicked, this, &TableEditor::onSaveToDB);
    connect(serializeToDBButton, &QPushButton::clicked, this, &TableEditor::onSerializeToDB);
}

QString TableEditor::getCurrentPureTableName() const
{
    return m_currentSelectedTablePureName;
}

void TableEditor::onSaveToDB()
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

void TableEditor::onSerializeToDB()
{
    if(parent->SerializeDB())
    {
        QMessageBox::information(this, tr("Cached Table"),
                             tr("Serialize Successful!"));
    }
    else
    {
        QMessageBox::warning(this, tr("Cached Table"),
                             tr("The database table(s) already exists permanently OR Something happened!"));
    }
}

void TableEditor::SetComboBox(const QString &tableName)
{
    if(combo)
    {
        RemoveComboBox();
    }
    combo = new QComboBox;
    combo->addItems({tableName + "_Data", tableName + "_Meta"});
    this->layout()->addWidget(combo);

    connect(combo, &QComboBox::currentTextChanged, this, &TableEditor::onCurrentTextChanged);
}

void TableEditor::RemoveComboBox()
{
    if(combo)
    {
        layout()->removeWidget(combo);
        disconnect(combo, &QComboBox::currentTextChanged, this, &TableEditor::onCurrentTextChanged);
        delete combo;
        combo = nullptr;
    }
}

void TableEditor::onCurrentTextChanged(const QString& table)
{
    if(parent)
    {
        SetModelView(parent->m_dbMan.GetStoreDB(), table);
        SetCustomLayout();
    }
}

void TableEditor::SetModelView(QSqlDatabase& db, const QString &tableName)
{
    if(tableName.contains('_'))
        m_currentSelectedTablePureName = tableName.split("_")[0];
    if(model)
    {
        delete model;
        disconnect(revertButton, &QPushButton::clicked,  model, &QSqlTableModel::revertAll);
    }
    model = new QSqlTableModel(this, db);
    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    bool x = model->select();

    //model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    //model->setHeaderData(1, Qt::Horizontal, tr("First name"));
    //model->setHeaderData(2, Qt::Horizontal, tr("Last name"));

    if(view)
        delete view;
    view = new QTableView;
    view->setModel(model);
    view->resizeColumnsToContents();
}

void TableEditor::SetCustomLayout()
{
    if(mainLayout)
        delete mainLayout;
    mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(view);
    mainLayout->addWidget(buttonBox);
    if(combo)
        mainLayout->addWidget(combo);
    setLayout(mainLayout);

    setWindowTitle(tr("Database Cached Table"));
}



