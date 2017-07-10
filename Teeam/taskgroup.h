#ifndef TASKGROUP_H
#define TASKGROUP_H

#include "ganttentity.h"
#include "generictask.h"

#include <QList>
#include <QColor>

class TaskGroup : public GanttEntity
{
    friend class TeeamProject;
public:
    TaskGroup(QString taskGroupName);
    QString getName();
    bool IsNew();

private:
    QDateTime end;
    QColor color;
    QList<GenericTask*> taskList;
    bool isNew;

protected:
    void setNew(bool b);

private:
    void AddTask(GenericTask* task);
};

#endif // TASKGROUP_H
