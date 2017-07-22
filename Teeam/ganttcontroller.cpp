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
    project = newProject;
    project->Show();
    project->SetNew(false);
}

void GanttController::EditProject(QString newName, QList<QString> newPeople)
{
    project->SetNew(false); // solo per sicurezza
    project->setName(newName);
    project->setPeopleList(newPeople);
}

void GanttController::AddTaskGroup(AbstractView *view, QString taskGroupName)
{
    TaskGroup *taskGroup = new TaskGroup(taskGroupName);
    taskGroup->attach(view);
    project->AddTaskGroup(taskGroup);
}

void GanttController::AddTask(AbstractView *view, QString taskName, QDateTime start, QDateTime end, QList<QString> taskPeople, int completition, int selectedParent)
{
    Task *task = new Task(taskName, start, end, taskPeople, completition);
    task->attach(view);
    if(selectedParent != -1)    
        project->AddTaskOrMilestoneToGroup(task, selectedParent);
    else
        project->AddTaskOrMilestone(task);
}

void GanttController::AddMilestone(AbstractView *view, QString milestoneName, QDateTime start, QList<QString> taskPeople, int selectedParent)
{
    Milestone *milestone = new Milestone(milestoneName, start, taskPeople);
    milestone->attach(view);
    if(selectedParent != -1)
        project->AddTaskOrMilestoneToGroup(milestone, selectedParent);
    else
        project->AddTaskOrMilestone(milestone);
}

void GanttController::SetFreeDays(Days days)
{
    this->freeDays->SetFreeDays(days);
}

void GanttController::SetFreeDaysColor(QColor color)
{
    this->freeDays->SetFreeDaysColor(color);
}

