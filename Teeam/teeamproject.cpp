#include "teeamproject.h"

TeeamProject::TeeamProject()
{

}

void TeeamProject::AddTaskGroup(TaskGroup *taskGroup)
{
    taskGroupList.append(taskGroup);
}
