#ifndef MILESTONE_H
#define MILESTONE_H


#include "ganttentity.h"
#include "generictask.h"

class Milestone : public GanttEntity, public GenericTask
{
public:
    Milestone();


public:
    void AddDependency(GenericTask* dependency);
};

#endif // MILESTONE_H
