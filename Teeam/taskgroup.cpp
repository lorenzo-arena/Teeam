#include "taskgroup.h"

TaskGroup::TaskGroup(QString taskGroupName)
{
    this->name = taskGroupName;
    this->isNew = true;
    this->changed = false;
    this->isRemoved = false;
    this->isGroupChanged = false;
}

TaskGroup::~TaskGroup()
{
    while(!taskList.empty())
    {
        delete taskList.takeAt(0);
    }
}

QString TaskGroup::GetName()
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

void TaskGroup::replaceEntity(int index, GenericTask *entity)
{
    if(index < taskList.length())
    {
        GenericTask* temp = taskList.at(index);
        taskList.replace(index, entity);
        delete temp;
    }
}

void TaskGroup::removeEntityAt(int index)
{
    delete taskList.takeAt(index);
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
