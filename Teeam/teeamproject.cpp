#include "teeamproject.h"

TeeamProject::TeeamProject(QString projectName, QList<QString> peopleList)
{
    this->name = projectName;
    this->peopleList = peopleList;
    this->bChanged = false;
    this->projectChanged = false;
    this->taskGroupChanged = false;
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

// TODO : trovare soluzione migliore??
void TeeamProject::Show()
{
    projectChanged = true;
    bChanged = true;
    notify();
    projectChanged = false;
    bChanged = false;
}

