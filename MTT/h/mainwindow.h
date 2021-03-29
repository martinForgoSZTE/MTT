#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "db_manager.h"

class QAction;
class QMenu;
class QStackedWidget;

class MapWidget;
class TableEditor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DB_Manager&) noexcept;
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

/*    void loadFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void about();

private:
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);*/

private slots:
    void open();
    //bool save();

private:
    void createActions();
    void createStatusBar();

    QString m_curFile;
    DB_Manager& m_dbMan;
    MapWidget* m_pMapWidget;
    TableEditor* m_pEditor;
};

#endif
