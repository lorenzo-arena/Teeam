#include "taskgroup.h"

TaskGroup::TaskGroup(QString taskGroupName)
{
    this->name = taskGroupName;
    this->isNew = true;
    this->bChanged = false;
    this->isRemoved = false;
    this->isGroupChanged = false;
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

void TaskGroup::ReplaceEntity(int index, GenericTask *entity)
{
    if(index < taskList.length())
    {
        taskList.replace(index, entity);
    }
}

void TaskGroup::setBeforeCreation()
{
    isNew = true;
    for (int i = 0; i < taskList.length(); i++)
        taskList.at(i)->setNew(true);
}

void TaskGroup::resetAfterCreation()
{
    isNew = false;
    for (int i = 0; i < taskList.length(); i++)
        taskList.at(i)->setNew(false);
}

void TaskGroup::searchAndRemovePerson(QString person)
{
    for(int index = 0; index < taskList.length(); index++)
    {
        taskList.at(index)->searchAndRemovePerson(person);
    }
}
