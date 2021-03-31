#include "mainwindow.h"
#include "mapwidget.h"
#include "table_editor_widget.h"
#include "db_modal_dialog.h"

#include <QtWidgets>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QStringList>


MainWindow::MainWindow(DB_Manager& db_manager) noexcept
    : m_dbMan(db_manager), m_pMapWidget(new MapWidget(this)), m_pEditor(nullptr)
{
    setCentralWidget(m_pMapWidget->GetGraphicsView());

    createActions();
    createStatusBar();
}

void MainWindow::FileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    QString tablename;
    if(fileName != "")
    {
        tablename = m_dbMan.CreateCustomDB(fileName);
        m_pEditor = new TableEditor(m_dbMan.GetStoreDB() , tablename + "_Data", this);
        setCentralWidget(m_pEditor);
    }
}

void MainWindow::DBOpen()
{
    auto tablesSet = m_dbMan.GetTables();
    QStringList tables(tablesSet.begin(), tablesSet.end());

    DBModal* modal = new DBModal(tables);
    if(modal->exec() && m_pEditor)
    {
        m_pEditor->SetComboBox(modal->GetSelectedTable());
        setCentralWidget(m_pEditor);
    }
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
    //connect(switchToMapAct, &QAction::triggered, this, /*&MainWindow::open*/);
    stagesMenu->addAction(switchToMapAct);

    stagesMenu->addSeparator();

    const QIcon tableIcon = QIcon::fromTheme("table");
    QAction *switchToTableAct = new QAction(tableIcon, tr("&Table view"), this);
    switchToTableAct->setStatusTip(tr("Open selected database table"));
    //connect(switchToTableAct, &QAction::triggered, this, /*&MainWindow::open*/);
    stagesMenu->addAction(switchToTableAct);

    stagesMenu->addSeparator();

    const QIcon chartsIcon = QIcon::fromTheme("chart");
    QAction *switchToChartsAct = new QAction(chartsIcon, tr("&Charts View"), this);
    switchToChartsAct->setStatusTip(tr("Open Charts view"));
    //connect(switchToChartsAct, &QAction::triggered, this, /*&MainWindow::open*/);
    stagesMenu->addAction(switchToChartsAct);



    /*QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));


    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));*/


}

MainWindow::~MainWindow()
{
    //TODO disconnects, connecting obj into data members
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}
