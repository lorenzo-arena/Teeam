#include "teeamproject.h"

TeeamProject::TeeamProject(QString projectName, QList<QString> peopleList)
{
    this->name = projectName;
    this->peopleList = peopleList;
    this->bChanged = false;
    this->projectChanged = false;
    this->taskGroupChanged = false;
	this->entitiesListChanged = false;
}

void TeeamProject::setName(QString projectName)
{
    this->name = projectName;
    projectChanged = true;
    bChanged = true;
    notify();
    projectChanged = false;
    bChanged = false;
}

void TeeamProject::setPeopleList(QList<QString> peopleList)
{
    this->peopleList = peopleList;
    projectChanged = true;
    bChanged = true;
    notify();
    projectChanged = false;
    bChanged = false;
}

void TeeamProject::AddTaskGroup(TaskGroup *taskGroup)
{
    taskGroupList.append(taskGroup);
    taskGroupChanged = true;
    bChanged = true;
    notify();
    taskGroupChanged = false;
    bChanged = false;
    taskGroup->setNew(false);
}

void TeeamProject::AddTaskOrMilestone(GenericTask *entity)
{
    entitiesList.append(entity);
    entitiesListChanged = true;
    bChanged = true;
    notify();
    entitiesListChanged = false;
    bChanged = false;
    entity->setNew(false);
}

void TeeamProject::AddTaskOrMilestoneToGroup(GenericTask *entity, int groupIndex)
{
    taskGroupList.at(groupIndex - 1)->AddTask(entity);
    taskGroupList.at(groupIndex - 1)->setChanged(true);
    taskGroupChanged = true;
    bChanged = true;
    notify();
    taskGroupList.at(groupIndex - 1)->setChanged(false);
    taskGroupChanged = false;
    bChanged = false;
    entity->setNew(false);
}

// TODO : trovare soluzione migliore??
void TeeamProject::Show()
{
    projectChanged = true;
    bChanged = true;
    notify();
    projectChanged = false;
    bChanged = false;
}

void TeeamProject::RemoveTaskGroup(int index)
{
    if(index < taskGroupList.length())
    {
        taskGroupChanged = true;
        bChanged = true;
        taskGroupList.at(index)->setRemoved(true);
        notify();
        taskGroupChanged = false;
        bChanged = false;
        taskGroupList.removeAt(index);
    }
}

void TeeamProject::RemoveTaskOrMilestone(int index, int parent)
{
    if(parent == -1)
    {
        // è un task/milestone che non appartiene a nessun gruppo
        if(index < entitiesList.length())
        {
            entitiesListChanged = true;
            bChanged = true;
            entitiesList.at(index)->setRemoved(true);
            notify();
            entitiesListChanged = false;
            bChanged = false;
            entitiesList.removeAt(index);
        }
    }
    else if(parent < taskGroupList.length())
    {
        // è un task/milestone che non appartiene a nessun gruppo
        if(index < taskGroupList.at(parent)->GetEntitiesList().length())
        {
            taskGroupChanged = true;
            bChanged = true;
            taskGroupList.at(parent)->setChanged(true);
            taskGroupList.at(parent)->GetEntitiesList().at(index)->setRemoved(true);
            notify();
            taskGroupChanged = false;
            bChanged = false;
            taskGroupList.at(parent)->setChanged(false);
            taskGroupList.at(parent)->GetEntitiesList().removeAt(index);
        }
    }
}

