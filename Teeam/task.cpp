#include "task.h"

Task::Task()
{

}

void Task::AddDependency(GenericTask* dependency)
{
    dependecies.append(dependency);
}
