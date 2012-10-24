#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include "mainwindow.h"
#include "configwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Aten");
    //QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("Gmentohust");
    MainWindow *mainWD = new MainWindow;
    mainWD->show();

    return app.exec();
}
