/////
#include <iostream>
////

#include "mainwindow.h"
#include "db_manager.h"
#include "constants.h"

#include <QApplication>
#include <QtGlobal>

int main(int argc, char *argv[])
{
    DB_Manager db_man(SQLITE_DRIVER);
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/thesis_icon.png"));
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    MainWindow mainWin(db_man);
    mainWin.show();
    return app.exec();
}
