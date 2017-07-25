#include "teeamproject.h"

TeeamProject::TeeamProject(QString projectName, QList<QString> peopleList)
{
    this->name = projectName;
    this->peopleList = peopleList;
    this->bChanged = false;
    this->projectChanged = false;
    this->taskGroupChanged = false;
	this->entitiesListChanged = false;
    this->isNew = true;
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
    taskGroupChanged = true;
    bChanged = true;
    notify();
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

void TeeamProject::ShowGroups()
{
    taskGroupChanged = true;
    bChanged = true;
    notify();
    taskGroupChanged = false;
    bChanged = false;
}

