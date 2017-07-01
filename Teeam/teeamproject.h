#ifndef TEEAMPROJECT_H
#define TEEAMPROJECT_H

#include "ganttentity.h"
#include "taskgroup.h"

#include <QList>

class TeeamProject: public GanttEntity
{
public:
    TeeamProject();

private:
    QList<TaskGroup*> taskGroupList;

public:
    void AddTaskGroup(TaskGroup* taskGroup);
};

#endif // TEEAMPROJECT_H
