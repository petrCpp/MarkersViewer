#include "mainwindow.h"
#include <QApplication>
#include <AppController.h>
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AppController appController;

    return a.exec();
}
