#ifndef TASK_H
#define TASK_H

#include "ganttentity.h"
#include "generictask.h"

class Task : public GanttEntity, public GenericTask
{
public:
    Task();


public:
    void AddDependency(GenericTask* dependency);
};

#endif // TASK_H
