#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "db_manager.h"
#include "table_editor_widget.h"
#include "mtt_Coordinate.h"
#include "mtt_basechart.h"


class QAction;
class QMenu;
class QStackedWidget;
class ChartsManager;
class QComboBox;

class MapWidget;

class MainWindow : public QMainWindow
{
    //TODO signal - slot?
    friend void TableEditor::onCurrentTextChanged(const QString&);
    Q_OBJECT

public:
    explicit MainWindow(DB_Manager&) noexcept;
    ~MainWindow();

    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

    bool SerializeDB() const
    {
        return m_dbMan.SerializeDB();
    }

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
    void FileOpen();
    void DBOpen();
    void onSwitchToMap();
    void onSwitchToTable();
    void onSwitchToCharts();
    void responseToSetChartData(const QVector<Coordinate>& selectedCoords, const QString& year);
    void responseToSetChartData(const QVector<Coordinate>& selectedCoords, const QString& startInterval, const QString& endInterval);
    //bool save();

private:
    void createActions();
    void createStatusBar();
    void AddItemsToScene(const QVector<Coordinate>& coords);

    QString m_curFile;
    DB_Manager& m_dbMan;
    MapWidget* m_pMapWidget;
    TableEditor* m_pEditor;
    QStackedWidget* m_pStackWidget;

    ChartsManager* m_pChartsManager;

    QComboBox* m_pChartTypesCombo;
};

#endif
