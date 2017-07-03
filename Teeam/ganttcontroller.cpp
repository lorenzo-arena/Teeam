#include "ganttcontroller.h"

GanttController::GanttController(FreeDaysModel *freeDays)
{
    this->freeDays = freeDays;
}

void GanttController::AddProject()
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

