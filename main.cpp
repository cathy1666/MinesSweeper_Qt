#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    MainWindow *MainWnd = new MainWindow;
    // Set window background color
    MainWnd->setStyleSheet("MainWindow { background-color: #E6E6E6; }");

    MainWnd->gameReset();
    MainWnd->show();
    return a.exec();
}
