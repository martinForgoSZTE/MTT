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
#include <QDir>
#include <QSqlDatabase>
#include <QHeaderView>
#include <QScrollArea>
#include <QFormLayout>
#include <QGroupBox>


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
      mainLayout(nullptr)
{
    view = new QTableView;
    saveDBButton = new QPushButton(tr("Submit"));
    saveDBButton->setDefault(true);
    revertButton = new QPushButton(tr("&Revert"));
    serializeToDBButton = new QPushButton(tr("Serialize"));

    settingsContentWidget = new QWidget();

    settingsLayout = new QFormLayout(settingsContentWidget);
    settingsLayout->addWidget(saveDBButton);
    settingsLayout->addWidget(revertButton);
    settingsLayout->addWidget(serializeToDBButton);
    QGroupBox* settings = new QGroupBox("Settings");
    settings->setLayout(settingsLayout);

    QVBoxLayout* scrollLayout = new QVBoxLayout();
    scrollLayout->addWidget(settings);

    settingsContentWidget->setLayout(scrollLayout);
    settingsContentWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QWidget* outmostWidget = new QWidget();
    vLayout = new QVBoxLayout();
    vLayout->addWidget(settingsContentWidget);
    outmostWidget->setLayout(vLayout);

    mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(view);
    mainLayout->addWidget(outmostWidget);
    setLayout(mainLayout);

    connect(saveDBButton, &QPushButton::clicked, this, &TableEditor::onSaveToDB);
    connect(serializeToDBButton, &QPushButton::clicked, this, &TableEditor::serializeToDB);
    setWindowTitle(tr("Database Cached Table"));
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

void TableEditor::SetComboBox(const QString &tableName)
{
    if(combo && comboSettings)
    {
        RemoveComboBox();
    }
    combo = new QComboBox;
    combo->addItems({tableName + "_Data", tableName + "_Meta"});
    QFormLayout* comboLayout = new QFormLayout(settingsContentWidget);
    comboLayout->addRow("Select a Table: ", combo);
    comboSettings = new QGroupBox("Tables");
    comboSettings->setLayout(comboLayout);
    comboSettings->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    vLayout->addWidget(comboSettings);

    connect(combo, &QComboBox::currentTextChanged, this, &TableEditor::currentTableChanged);
}

void TableEditor::RemoveComboBox()
{
    if(combo && comboSettings)
    {
        vLayout->removeWidget(comboSettings);
        delete comboSettings;
        comboSettings = nullptr;
        combo = nullptr;
    }
}

void TableEditor::SetModelView(QSqlDatabase& db, const QString &tableName)
{
    if(tableName.contains('_'))
        m_currentSelectedTablePureName = tableName.split("_")[0];
    if(model)
    {
        delete model;
    }
    model = new QSqlTableModel(this, db);
    connect(revertButton, &QPushButton::clicked, model, &QSqlTableModel::revertAll);
    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    bool x = model->select();

    if (tableName.split("_")[1] == "Data")
    {
        model->setHeaderData(0, Qt::Horizontal, tr("Area"));
        model->setHeaderData(1, Qt::Horizontal, tr("Year"));
        model->setHeaderData(2, Qt::Horizontal, tr("Data"));
    }
    else if (tableName.split("_")[1] == "Meta")
    {
        model->setHeaderData(0, Qt::Horizontal, tr("Area"));
        model->setHeaderData(1, Qt::Horizontal, tr("Capital"));
        model->setHeaderData(2, Qt::Horizontal, tr("County"));
        model->setHeaderData(3, Qt::Horizontal, tr("Region"));
        model->setHeaderData(4, Qt::Horizontal, tr("Large Region"));
    }

    view->setModel(model);
    view->resizeColumnsToContents();
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}



