#include "taskgroup.h"

TaskGroup::TaskGroup(QColor color)
{
    this->color = color;
}

void TaskGroup::AddTask(GenericTask *task)
{
    taskList.append(task);
}
