/////
#include <iostream>
////

#include "MTT.h"
#include "mainwindow.h"
#include "db_manager.h"

#include <QApplication>
#include <QtGlobal>

int main(int argc, char *argv[])
{
    DB_Manager db_man("QSQLITE");
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("MTT - Szakdoga app");

    MainWindow mainWin(db_man);
    mainWin.show();
    return app.exec();
}
