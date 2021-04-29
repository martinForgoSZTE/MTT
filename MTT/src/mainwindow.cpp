#include "mainwindow.h"
#include "mapwidget.h"
#include "table_editor_widget.h"
#include "db_modal_dialog.h"
#include "circle_graphics_item.h"
#include "mtt_charts.h"

#include <QtWidgets>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QStringList>
#include <QMessageBox>
#include <QVector>
#include <QPalette>


MainWindow::MainWindow(DB_Manager& db_manager) noexcept
    : m_dbMan(db_manager), m_pMapWidget(new MapWidget(this)), m_pEditor(nullptr), m_pChartsManager(new ChartsManager(this))
{
    m_pStackWidget = new QStackedWidget(this);
    m_pStackWidget->addWidget(m_pMapWidget->GetGraphicsView());

    setCentralWidget(m_pStackWidget);

    createActions();
    createStatusBar();
}

void MainWindow::FileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    QString tablename;
    if(fileName != "")
    {
        m_dbMan.SetStoreDB();

        tablename = m_dbMan.CreateCustomDB(fileName);
        if(!m_pEditor)
        {
            m_pEditor = new TableEditor( this);
            m_pStackWidget->insertWidget(m_pStackWidget->count()+1, m_pEditor);
        }
        else
            m_pEditor->RemoveComboBox();

        m_pEditor->SetModelView(m_dbMan.GetStoreDB() , tablename + "_Data");
        m_pEditor->SetCustomLayout();

        AddItemsToScene(m_dbMan.GetMappedCoordinates(tablename + "_Meta"));
    }
}

void MainWindow::DBOpen()
{
    m_dbMan.SetStoreDB(STORE_DB_PATH);

    auto tablesSet = m_dbMan.GetTables(m_dbMan.GetStoreDB());
    QStringList tables(tablesSet.begin(), tablesSet.end());

    DBModal* modal = new DBModal(tables);
    if(modal->exec())
    {
        if(!m_pEditor)
        {
            m_pEditor = new TableEditor( this);
            m_pStackWidget->insertWidget(m_pStackWidget->count()+1, m_pEditor);
        }
        m_pEditor->SetModelView(m_dbMan.GetStoreDB() , tables[0] + "_Data");
        m_pEditor->SetCustomLayout();
        m_pEditor->SetComboBox(modal->GetSelectedTable());

        AddItemsToScene(m_dbMan.GetMappedCoordinates(tables[0] + "_Meta"));
    }
}

void MainWindow::AddItemsToScene(const QVector<Coordinate>& coords)
{
    auto* scene = m_pMapWidget->GetGraphicsScene();
    for(auto& coord : coords)
    {
        scene->addItem(new CircleGraphicsItem(coord));
    }
}

void MainWindow::createActions()
{

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    QMenu* subOpenMenu = fileMenu->addMenu(tr("&Open"));
    QMenu *stagesMenu = menuBar()->addMenu(tr("&Views"));

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));

    subOpenMenu->setIcon(openIcon);

    QAction *fileOpenAct = new QAction(openIcon, tr("&Open file..."), this);
    fileOpenAct->setShortcuts(QKeySequence::Open);
    fileOpenAct->setStatusTip(tr("Open a STAT file"));
    connect(fileOpenAct, &QAction::triggered, this, &MainWindow::FileOpen);
    subOpenMenu->addAction(fileOpenAct);
    fileToolBar->addAction(fileOpenAct);

    QAction *dbOpenAct = new QAction(openIcon, tr("&Open database..."), this);
    dbOpenAct->setShortcuts(QKeySequence::Open);
    dbOpenAct->setStatusTip(tr("Open a STAT file"));
    connect(dbOpenAct, &QAction::triggered, this, &MainWindow::DBOpen);
    subOpenMenu->addAction(dbOpenAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save data into DB"));
    //connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    fileMenu->addSeparator();

    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));

    const QIcon mapIcon = QIcon::fromTheme("map");
    QAction *switchToMapAct = new QAction(mapIcon, tr("&Map view"), this);
    switchToMapAct->setStatusTip(tr("Open Map View"));
    connect(switchToMapAct, &QAction::triggered, this, &MainWindow::onSwitchToMap);
    stagesMenu->addAction(switchToMapAct);

    stagesMenu->addSeparator();

    const QIcon tableIcon = QIcon::fromTheme("table");
    QAction *switchToTableAct = new QAction(tableIcon, tr("&Table view"), this);
    switchToTableAct->setStatusTip(tr("Open selected database table"));
    connect(switchToTableAct, &QAction::triggered, this, &MainWindow::onSwitchToTable);
    stagesMenu->addAction(switchToTableAct);

    stagesMenu->addSeparator();

    const QIcon chartsIcon = QIcon::fromTheme("chart");
    QAction *switchToChartsAct = new QAction(chartsIcon, tr("&Charts View"), this);
    switchToChartsAct->setStatusTip(tr("Open Charts view"));
    connect(switchToChartsAct, &QAction::triggered, this, &MainWindow::onBeforeSwitchToCharts);
    stagesMenu->addAction(switchToChartsAct);



    /*QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));


    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));*/

    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    fileToolBar->addWidget(empty);

    m_pChartTypesCombo = new QComboBox();
    m_pChartTypesCombo->setPlaceholderText("Select a Chart!");
    m_pChartTypesCombo->addItem("PieChart", CHART_TYPES::PIECHART);
    m_pChartTypesCombo->addItem("OtherChart");
    fileToolBar->addWidget(m_pChartTypesCombo);

    QWidget* empty2 = new QWidget();
    empty2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    fileToolBar->addWidget(empty2);

}

MainWindow::~MainWindow()
{
    //TODO disconnects, connecting obj into data members
}

void MainWindow::onSwitchToMap()
{
    if(m_pMapWidget->GetGraphicsView())
        m_pStackWidget->setCurrentWidget(m_pMapWidget->GetGraphicsView());
    else
        QMessageBox::warning(this, tr("Map Warning"), tr("Map isn't loaded yet!"));
}

void MainWindow::onSwitchToTable()
{
    if(m_pEditor)
        m_pStackWidget->setCurrentWidget(m_pEditor);
    else
        QMessageBox::warning(this, tr("DB Warning"), tr("Data isn't loaded yet!"));
}

void MainWindow::onBeforeSwitchToCharts()
{
    //TODO lekérni a kiválasztott koordinátákhoz tartozó értékeket (adott évre [a legelsőre defaultból])
    //ezeket továbbadni onSwitchToChartsnak
}

void MainWindow::onSwitchToCharts(const QVector<Coordinate>& coords)
{
    CHART_TYPES type = CHART_TYPES::UNKNOWN;
    if(m_pChartTypesCombo->currentData(m_pChartTypesCombo->currentIndex()).isValid())
    {
        type = static_cast<CHART_TYPES>(m_pChartTypesCombo->currentData(m_pChartTypesCombo->currentIndex()).toInt());

        if(m_pChartsManager)
        {
            if(m_pStackWidget->indexOf(m_pChartsManager->GetChart()->get()) != -1)
            {
                m_pStackWidget->removeWidget(m_pChartsManager->GetChart()->get());
            }
            m_pChartsManager->setChart(type, coords);
            m_pStackWidget->insertWidget(m_pStackWidget->count() + 1, m_pChartsManager->GetChart()->get());
            m_pStackWidget->setCurrentWidget(m_pChartsManager->GetChart()->get());
        }
    }
    else
        QMessageBox::warning(this, tr("Charts Warning"), tr("Charts not selected yet!"));
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}


/*void MainWindow::newFile()
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile(QString());
    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}*/
