#include "ganttcontroller.h"

GanttController::GanttController(FreeDaysModel *freeDays, TeeamProject *project)
{
    // Aggiungere puntatori task + milestone
    this->freeDays = freeDays;

    this->project = project;
}

void GanttController::NewProject(AbstractView *view, TeeamProject *project, QString projectName)
{
    this->project = project;
    this->project->attach(view);
    project->setName(projectName);
}

void GanttController::AddTaskGroup(AbstractView *view, QString taskGroupName)
{
    TaskGroup *taskGroup = new TaskGroup(taskGroupName);
    taskGroup->attach(view);
    project->AddTaskGroup(taskGroup);
}

void GanttController::AddTask(AbstractView *view, QString taskName, QDateTime start, QDateTime end, QList<QString> taskPeople, int selectedParent)
{
    Task *task = new Task(taskName, start, end, taskPeople);
    task->attach(view);
    if(selectedParent != -1)    
        project->GetTaskGroup().at(selectedParent-1)->AddTask(task);
    else
        project->AddTaskOrMilestone(task);
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

