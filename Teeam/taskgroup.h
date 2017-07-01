#ifndef TASKGROUP_H
#define TASKGROUP_H

#include "generictask.h"

#include <QList>
#include <QColor>

class TaskGroup
{
public:
    TaskGroup(QColor color);

private:
    QColor color;
    QList<GenericTask*> taskList;

private:
    void AddTask(GenericTask* task);
};

#endif // TASKGROUP_H
