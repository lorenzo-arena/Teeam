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
    TeeamProject(QString projectName, QList<QString> peopleList);

private:
    QList<TaskGroup*> taskGroupList;
    QList<GenericTask *> entitiesList;
    QList<QString> peopleList;
    bool taskGroupChanged;
    bool projectChanged;
    bool entitiesListChanged;

public:
    void AddTaskGroup(TaskGroup* taskGroup);
    void AddTaskOrMilestone(GenericTask *entity);
    QList<GenericTask *> GetEntitiesList() { return entitiesList; }
    QList<TaskGroup *> GetTaskGroup() { return taskGroupList; }
    bool IsTaskGroupChanged() { return taskGroupChanged; }
    bool IsProjectChanged() { return projectChanged; }
	bool IsEntitiesListChanged() { return entitiesListChanged; }
    QString GetName() { return name; }
    QList<QString> GetPeopleList() { return peopleList; }
    void setName(QString projectName);
    void setPeopleList(QList<QString> peopleList);
    void Show();
};

#endif // TEEAMPROJECT_H
