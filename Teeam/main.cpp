#include "mainwindow.h"
#include "freedaysmodel.h"
#include "teeamproject.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setApplicationVersion(APP_VERSION);
    qApp->setApplicationName(QString(APP_NAME).replace("_", " "));
    qApp->setOrganizationName("Teeam");
    qApp->setOrganizationDomain("");

    FreeDaysModel freeDays;

    // TODO : add project initialization

    QString projectFilePath = "";

    // Sto aprendo un file con il programma
    if(argc == 2)
    {
        // Prelevo il percorso del file
        projectFilePath = a.arguments().at(1);
    }

    GanttController ganttController(&freeDays);//, nullptr);
    MainWindow w(&ganttController, &freeDays, "v" + qApp->applicationVersion(), projectFilePath);
    w.setWindowTitle(qApp->applicationName() + " v" + qApp->applicationVersion());
    w.show();

    return a.exec();
}
