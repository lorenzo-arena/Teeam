#include "milestone.h"

Milestone::Milestone()
{

}

void Milestone::AddDependency(GenericTask* dependency)
{
    dependecies.append(dependency);
}
