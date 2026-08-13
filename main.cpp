#include <QCoreApplication>
#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Mainwindow mainWin;

    return a.exec();
}
