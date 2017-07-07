#include "teeamproject.h"

TeeamProject::TeeamProject()
{
    bChanged = false;
}

void TeeamProject::setName(QString projectName)
{
    this->name = projectName;
    bChanged = true;
    notify();
}


QString TeeamProject::getName()
{
    return name;
}

void TeeamProject::AddTaskGroup(TaskGroup *taskGroup)
{
    taskGroupList.append(taskGroup);
}
