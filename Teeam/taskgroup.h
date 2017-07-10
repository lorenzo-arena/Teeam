#ifndef TASKGROUP_H
#define TASKGROUP_H

#include "ganttentity.h"
#include "generictask.h"

#include <QList>
#include <QColor>

class TaskGroup : public GanttEntity
{
public:
    TaskGroup(QString taskGroupName);
    QString getName();

private:
    QDateTime end;
    QColor color;
    QList<GenericTask*> taskList;

private:
    void AddTask(GenericTask* task);
};

#endif // TASKGROUP_H
