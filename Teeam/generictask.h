#ifndef GENERICTASK_H
#define GENERICTASK_H

#include "ganttentity.h"
#include "teeamproject.h"

#include <QColor>
#include <QList>

typedef enum
{
	No_type,
    Task_type,
    Milestone_type
} EntityType;

class TeeamProject;

class GenericTask : public GanttEntity
{
    friend class TeeamProject;
    friend class TaskGroup;
public:
    GenericTask(){ isNew = true; changed = false; isRemoved = false; entityType = No_type; }
    bool IsNew() { return isNew; }
    EntityType getEntityType() { return entityType; }

protected:
    void setNew(bool b) { isNew = b; }
    void setRemoved(bool b) { isRemoved = b; }
    void searchAndRemovePerson(QString person)
    {
        for(int index = 0; index < people.length(); index++)
        {
            if(people.at(index) == person)
                people.removeAt(index);
        }
    }

    QColor color;
    QList<GenericTask*> dependecies;
    EntityType entityType; // La uso per distinguere tra Task e Milestone

public:
    virtual void AddDependency(GenericTask* dependency) = 0;
};

#endif // GENERICTASK_H
