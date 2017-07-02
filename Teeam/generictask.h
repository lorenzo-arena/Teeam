#ifndef GENERICTASK_H
#define GENERICTASK_H

#include "ganttentity.h"

#include <QColor>
#include <QList>

class GenericTask : public GanttEntity
{
public:
    GenericTask(){}

protected:
    QColor color;
    QList<GenericTask*> dependecies;

public:
    virtual void AddDependency(GenericTask* dependency) = 0;
};

#endif // GENERICTASK_H
