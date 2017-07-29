#ifndef TEEAMPROJECT_H
#define TEEAMPROJECT_H

#include "ganttentity.h"
#include "abstractview.h"
#include "taskgroup.h"
#include "generictask.h"

#include <QList>

class GenericTask;
class TaskGroup;

class TeeamProject: public GanttEntity
{
    friend class GenericTask;
    friend class TaskGroup;
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
    void AddTaskOrMilestoneToGroup(GenericTask *entity, int groupIndex);
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
    bool IsNew() { return isNew; }
    void SetNew(bool b) { isNew = b; }
    void ShowGroups();
    void RemoveTaskGroup(int index);
    void RemoveTaskOrMilestone(int index, int parent);

    // TODO : implementare metodi edit (potrebbe essere anche il controller a modificare
    //        i parametri), che si occupa di applicare changed; e entities con changed o new
    //        vengono ridisegnati allo stesso modo, ma se è solo changed si evita di assegnare una
    //        riga
};

#endif // TEEAMPROJECT_H
