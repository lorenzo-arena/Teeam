#include "milestone.h"

Milestone::Milestone(QString milestoneName, QDateTime start, QList<QString> taskPeople)
    : GenericTask()
{
    this->entityType = Milestone_type;
    this->name = milestoneName;
    this->start = start;
    this->people = taskPeople;
}

void Milestone::edit(QString milestoneName, QDateTime start, QList<QString> taskPeople)
{
    this->name = milestoneName;
    this->start = start;
    this->people = taskPeople;
}

void Milestone::AddDependency(GenericTask* dependency)
{
    dependecies.append(dependency);
}
