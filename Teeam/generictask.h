#ifndef GENERICTASK_H
#define GENERICTASK_H

#include "ganttentity.h"
#include "teeamproject.h"

#include <QColor>
#include <QList>

#define TASK_CODE           0x01
#define MILESTONE_CODE      0x02

class TeeamProject;

class GenericTask : public GanttEntity
{
    friend class TeeamProject;
public:
    GenericTask(){ isNew = true; }
    bool IsNew() { return isNew; }
    int getEntityType() { return entityType; }

protected:
    void setNew(bool b) { isNew = b; }
    QColor color;
    QList<GenericTask*> dependecies;
    QList<QString> people;
    int entityType; // La uso per distinguere tra Task e Milestone

public:
    virtual void AddDependency(GenericTask* dependency) = 0;
};

#endif // GENERICTASK_H
