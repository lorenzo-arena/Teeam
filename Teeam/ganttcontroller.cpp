#include "ganttcontroller.h"

#include "abstractview.h"
#include "taskgroup.h"

GanttController::GanttController(FreeDaysModel *freeDays, TeeamProject *project)
{
    // Aggiungere puntatori task + milestone
    this->freeDays = freeDays;
    this->project = project;
}

void GanttController::NewProject(TeeamProject *newProject)
{
    this->project = newProject;
    this->project->Show();
}

void GanttController::AddTaskGroup(QString taskGroupName)
{
    TaskGroup *taskGroup = new TaskGroup(taskGroupName);
    project->AddTaskGroup(taskGroup);
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

