#include "taskgroup.h"

TaskGroup::TaskGroup(QString taskGroupName)
{
    this->name = taskGroupName;
    this->isNew = true;
}

QString TaskGroup::getName()
{
    return name;
}

bool TaskGroup::IsNew()
{
    return isNew;
}

void TaskGroup::setNew(bool b)
{
    isNew = b;
}

void TaskGroup::AddTask(GenericTask *task)
{
    taskList.append(task);
}
