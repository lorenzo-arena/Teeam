#ifndef GANTTCONTROLLER_H
#define GANTTCONTROLLER_H

#include "freedaysmodel.h"
#include "teeamproject.h"

#include <QColor>

class GanttController
{
public:
    GanttController(FreeDaysModel *freeDays, TeeamProject *project);

// controller interface
public:
    void NewProject(TeeamProject *project, QString projectName);
    void AddTaskGroup();
    void AddTask();
    void AddMilestone();
    void SetFreeDays(Days days);
    void SetFreeDaysColor(QColor color);

private:
    FreeDaysModel *freeDays;
    TeeamProject *project;


};

#endif // GANTTCONTROLLER_H
