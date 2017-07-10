#include "teeamproject.h"

TeeamProject::TeeamProject()
{
    bChanged = false;
    projectChanged = false;
    taskGroupChanged = false;
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

bool TeeamProject::isProjectChanged()
{
    return projectChanged;
}

QString TeeamProject::getName()
{
    return name;
}

void TeeamProject::AddTaskGroup(TaskGroup *taskGroup)
{
    taskGroupList.append(taskGroup);
    taskGroupChanged = true;
    bChanged = true;
    notify();
    taskGroupChanged = false;
    bChanged = false;
}

QList<TaskGroup *> TeeamProject::GetTaskGroup()
{
    return taskGroupList;
}

bool TeeamProject::isTaskGroupChanged()
{
    return taskGroupChanged;
}


