#ifndef TEEAMPROJECT_H
#define TEEAMPROJECT_H

#include "abstractview.h"
#include "ganttentity.h"
#include "taskgroup.h"

#include <QList>

class TeeamProject: public GanttEntity
{
    friend class TaskGroup;
public:
    TeeamProject(QString projectName, QList<QString> peopleList);

private:
    QList<TaskGroup*> taskGroupList;
    QList<QString> peopleList;
    bool taskGroupChanged;
    bool projectChanged;

public:
    void AddTaskGroup(TaskGroup* taskGroup);
    QList<TaskGroup *> GetTaskGroup() { return taskGroupList; }
    bool IsTaskGroupChanged() { return taskGroupChanged; }
    bool IsProjectChanged() { return projectChanged; }
    QString GetName() { return name; }
    QList<QString> GetPeopleList() { return peopleList; }
    void setName(QString projectName);
    void setPeopleList(QList<QString> peopleList);
    void Show();
};

#endif // TEEAMPROJECT_H
