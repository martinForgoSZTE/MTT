#include "MTT.h"
#include "mainwindow.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("MTT - Szakdoga app");

    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}
