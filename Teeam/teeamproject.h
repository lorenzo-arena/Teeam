#ifndef TEEAMPROJECT_H
#define TEEAMPROJECT_H

#include "ganttentity.h"
#include "abstractview.h"
#include "taskgroup.h"
#include "generictask.h"

#include <QList>

class TaskGroup;
class GenericTask;

class TeeamProject: public GanttEntity
{
    friend class TaskGroup;
    friend class GenericTask;
public:
    TeeamProject();

private:
    QList<TaskGroup*> taskGroupList;
    QList<GenericTask *> entitiesList;
    bool taskGroupChanged;
    bool projectChanged;
    bool entitiesListChanged;

public:
    void AddTaskGroup(TaskGroup* taskGroup);
    void AddTaskOrMilestone(GenericTask *entity);
    QList<TaskGroup *> GetTaskGroup() { return taskGroupList; }
    QList<GenericTask *> GetEntitiesList() { return entitiesList; }
    bool isTaskGroupChanged() { return taskGroupChanged; }
    bool isProjectChanged() { return projectChanged; }
    bool isEntitiesListChanged() { return entitiesListChanged; }

    QString getName();
    void setName(QString projectName);
};

#endif // TEEAMPROJECT_H
