#include "milestone.h"

Milestone::Milestone()
    : GenericTask()
{
    this->entityType = MILESTONE_CODE;
}

void Milestone::AddDependency(GenericTask* dependency)
{
    dependecies.append(dependency);
}
