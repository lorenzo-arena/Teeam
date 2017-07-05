#include "ganttcontroller.h"

GanttController::GanttController(FreeDaysModel *freeDays, TeeamProject *project)
{
    this->freeDays = freeDays;

    if(project != nullptr)
        this->project = project;
}

void GanttController::NewProject()
{
}

void GanttController::AddTaskGroup()
{
}

void GanttController::AddTask()
{
}

void GanttController::AddMilestone()
{
}

void GanttController::SetFreeDays(Days days)
{
    this->freeDays->SetFreeDays(days);
}

void GanttController::SetFreeDaysColor(QColor color)
{
    this->freeDays->SetFreeDaysColor(color);
}

