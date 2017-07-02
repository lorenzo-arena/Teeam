#ifndef GANTTCONTROLLER_H
#define GANTTCONTROLLER_H

#include "abstractcontroller.h"

class GanttController
{
public:
    GanttController();

// controller interface
public:
    bool AddProject();
    bool AddTaskGroup();
    bool AddTask();
    bool AddMilestone();

};

#endif // GANTTCONTROLLER_H
