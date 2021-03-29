#include "mainwindow.h"
#include "mapwidget.h"
#include "table_editor_widget.h"

#include <QtWidgets>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStackedWidget>
#include <QVBoxLayout>


MainWindow::MainWindow(DB_Manager& db_manager) noexcept
    : m_dbMan(db_manager), m_pMapWidget(new MapWidget(this)), m_pEditor(nullptr)
{
    setCentralWidget(m_pMapWidget->GetGraphicsView());

    createActions();
    createStatusBar();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if(fileName != "")
        m_dbMan.CreateCustomDB("QSQLITE", fileName);
    m_pEditor = new TableEditor("Test_Data", this);
    setCentralWidget(m_pEditor->GetTableView());
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

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

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


    QMenu *stagesMenu = menuBar()->addMenu(tr("&Views"));

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

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}
