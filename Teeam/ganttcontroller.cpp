#include "ganttcontroller.h"

GanttController::GanttController(FreeDaysModel *freeDays, TeeamProject *project)
{
    // Aggiungere puntatori task + milestone
    this->freeDays = freeDays;

    this->project = project;
}

void GanttController::NewProject(TeeamProject *project, QString projectName)
{
    this->project = project;
    project->setName(projectName);
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

