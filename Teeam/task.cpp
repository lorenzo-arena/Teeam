#include "task.h"

Task::Task(QString taskName, QDateTime start, QDateTime end, QList<QString> taskPeople)
    : GenericTask()
{
    this->entityType = TASK_CODE;
    this->name = taskName;
    this->start = start;
    this->end = end;
    this->people = taskPeople;
}

void Task::AddDependency(GenericTask* dependency)
{
    dependecies.append(dependency);
}
