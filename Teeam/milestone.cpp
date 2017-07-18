#include "milestone.h"

Milestone::Milestone(QString milestoneName, QDateTime start, QList<QString> taskPeople)
    : GenericTask()
{
    this->entityType = MILESTONE_CODE;
    this->name = milestoneName;
    this->start = start;
    this->people = taskPeople;
}

void Milestone::AddDependency(GenericTask* dependency)
{
    dependecies.append(dependency);
}
