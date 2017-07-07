#include "mainwindow.h"
#include "freedaysmodel.h"
#include "teeamproject.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setApplicationVersion(APP_VERSION);
    qApp->setApplicationName(QString(APP_NAME).replace("_", " "));
    qApp->setOrganizationName("Teeam");
    qApp->setOrganizationDomain("");

    FreeDaysModel freeDays;
    TeeamProject project;

    // TODO : add project initialization

    GanttController ganttController(&freeDays, &project);

    MainWindow w(&ganttController, &freeDays, &project);
    w.setWindowTitle(qApp->applicationName() + " " + qApp->applicationVersion()); // TODO : controllare??
    w.show();

    return a.exec();
}
