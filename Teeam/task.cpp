#include "task.h"

Task::Task(QString taskName, QDateTime start, QDateTime end, QList<QString> taskPeople, int completition)
    : GenericTask()
{
    this->entityType = TASK_CODE;
    this->name = taskName;
    this->start = start;
    this->end = end;
    this->people = taskPeople;
    this->completition = completition;
}

void Task::edit(QString taskName, QDateTime start, QDateTime end, QList<QString> taskPeople, int completition)
{
    this->name = taskName;
    this->start = start;
    this->end = end;
    this->people = taskPeople;
    this->completition = completition;
}

void Task::AddDependency(GenericTask* dependency)
{
    dependecies.append(dependency);
}
