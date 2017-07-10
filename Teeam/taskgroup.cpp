#include "taskgroup.h"

TaskGroup::TaskGroup(QString taskGroupName)
{
    this->name = taskGroupName;
}

QString TaskGroup::getName()
{
    return this->name;
}

void TaskGroup::AddTask(GenericTask *task)
{
    taskList.append(task);
}
