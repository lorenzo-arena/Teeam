#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setApplicationVersion(APP_VERSION);
    qApp->setApplicationName(QString(APP_NAME).replace("_", " "));
    qApp->setOrganizationName("Teeam");
    qApp->setOrganizationDomain("");

    MainWindow w;
    w.setWindowTitle(qApp->applicationName() + " " + qApp->applicationVersion());
    w.show();

    return a.exec();
}
