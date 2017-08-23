#ifndef GENERICTASK_H
#define GENERICTASK_H

#include "ganttentity.h"
#include "teeamproject.h"

#include <QColor>
#include <QList>

typedef enum
{
    Task_type,
    Milestone_type
} EntityType;

class TeeamProject;

class GenericTask : public GanttEntity
{
    friend class TeeamProject;
public:
    GenericTask(){ isNew = true; bChanged = false; isRemoved = false; }
    bool IsNew() { return isNew; }
    EntityType getEntityType() { return entityType; }

protected:
    void setNew(bool b) { isNew = b; }
    void setRemoved(bool b) { isRemoved = b; }
    QColor color;
    QList<GenericTask*> dependecies;
    QList<QString> people;
    EntityType entityType; // La uso per distinguere tra Task e Milestone

public:
    virtual void AddDependency(GenericTask* dependency) = 0;
};

#endif // GENERICTASK_H
