#include "taskgroup.h"

TaskGroup::TaskGroup(QString taskGroupName)
{
    this->name = taskGroupName;
    this->isNew = true;
    this->bChanged = false;
    this->isRemoved = false;
}

QString TaskGroup::getName()
{
    return name;
}

bool TaskGroup::IsNew()
{
    return isNew;
}

void TaskGroup::AddTask(GenericTask *task)
{
    taskList.append(task);    
}
