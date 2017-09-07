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
    bool bSilentMode;

    // identificativi per file salvataggio
    const QString KEY_PROJECT = "teeamproject";
    const QString KEY_PROJECTPARAMETERS = "projectparameters";
    const QString KEY_NAME = "name";
    const QString KEY_PERSON = "person";
    const QString KEY_GROUP = "group";
    const QString KEY_ENTITY = "entity";
    const QString KEY_ENTITYTYPE = "entitytype";
    const QString KEY_TASKTYPE = "task";
    const QString KEY_MILESTONETYPE = "milestone";
    const QString KEY_STARTDATETIME = "startdatetime";
    const QString KEY_ENDDATETIME = "enddatetime";
    const QString KEY_COMPLETITION = "completition";


    void SetBeforeCreation();
    void ResetAfterCreation();

public:
    void AddTaskGroup(TaskGroup* taskGroup);
    void AddTaskOrMilestone(GenericTask *entity);
    void AddTaskOrMilestoneToGroup(GenericTask *entity, int groupIndex);
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
    void ShowEntities();
    void RemoveTaskGroup(int index);
    void RemoveTaskOrMilestone(int index, int parent);
    void EditTaskOrMilestone(GenericTask *entity, int index, int parent = -1);
    GenericTask * GetEntityAt(int index) { return entitiesList.at(index); }
    int GetEntitiesListSize() { return entitiesList.length(); }
    TaskGroup * GetTaskGroupAt(int index) { return taskGroupList.at(index); }
    int GetTaskGroupListSize() { return taskGroupList.length(); }
    void SaveProjectAs(const QString filename);
    int OpenFile(const QString filename, AbstractView *view);
};

#endif // TEEAMPROJECT_H
