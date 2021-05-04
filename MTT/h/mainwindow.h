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
class QTextEdit;

class MapWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DB_Manager&) noexcept;
    ~MainWindow();

    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

private slots:
    void onAboutThesis();
    void onAboutLicense();
    void FileOpen();
    void DBOpen();
    void onSwitchToMap();
    void onSwitchToTable();
    void onSwitchToCharts();
    void responseToSetChartData(const QVector<Coordinate>& selectedCoords, const QString& year);
    void responseToSetChartData(const QVector<Coordinate>& selectedCoords, const QString& startInterval, const QString& endInterval);
    void onSerializeToDB();
    void onCurrentTableChanged(const QString& newTableName);

private:
    void setUpGlobalConnections();
    void tearDownGlobalConnections();
    void createActions();
    void createStatusBar();

    QString m_curFile;
    DB_Manager& m_dbMan;
    MapWidget* m_pMapWidget;
    TableEditor* m_pEditor;
    QStackedWidget* m_pStackWidget;

    ChartsManager* m_pChartsManager;

    QComboBox* m_pChartTypesCombo;
};

#endif
