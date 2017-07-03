#ifndef GANTTCONTROLLER_H
#define GANTTCONTROLLER_H

#include "freedaysmodel.h"

#include <QColor>

class GanttController
{
public:
    GanttController(FreeDaysModel *freeDays);

// controller interface
public:
    void AddProject();
    void AddTaskGroup();
    void AddTask();
    void AddMilestone();
    void SetFreeDays(Days days);
    void SetFreeDaysColor(QColor color);

private:
    FreeDaysModel *freeDays;

};

#endif // GANTTCONTROLLER_H
