#ifndef TASKGROUP_H
#define TASKGROUP_H

#include "ganttentity.h"
#include "generictask.h"
#include "teeamproject.h"

#include <QList>
#include <QColor>

class GenericTask;
class TeeamProject;

class TaskGroup : public GanttEntity
{
    friend class GenericTask;
    friend class TeeamProject;
public:
    TaskGroup(QString taskGroupName);
    QString getName();
    bool IsNew();
    void AddTask(GenericTask* task);
    QList<GenericTask*> GetEntitiesList() { return taskList; }

private:
    QDateTime end;
    QColor color;
    QList<GenericTask*> taskList;
    bool isNew;

protected:
    void setNew(bool b) { isNew = b; }

};

#endif // TASKGROUP_H
