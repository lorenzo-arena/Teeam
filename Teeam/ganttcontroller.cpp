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

void GanttController::EditTaskGroup(int index, QString newName)
{
    if(index-1 < project->GetTaskGroup().length())
    {
        project->GetTaskGroup().at(index-1)->setName(newName);
        project->ShowGroups();
    }
}

void GanttController::EditTaskOrMilestone(QString entityName, QDateTime start, QDateTime end, QList<QString> taskPeople, int completition, int selectedParent, int index, int parent)
{
    if(parent == -1)
    {
        // è un task che non appartiene a nessun gruppo
        Task *task = new Task(entityName, start, end, taskPeople, completition);
        project->EditTaskOrMilestone(task, index);
    }
}

void GanttController::EditTaskOrMilestone(QString entityName, QDateTime start, QList<QString> milestonePeople, int selectedParent, int index, int parent)
{
    if(parent == -1)
    {
        // è una milestone che non appartiene a nessun gruppo
        Milestone *milestone = new Milestone(entityName, start, milestonePeople);
        project->EditTaskOrMilestone(milestone, index);
    }
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

void GanttController::RemoveTaskGroup(int index)
{
    project->RemoveTaskGroup(index);
}

void GanttController::RemoveTaskOrMilestone(int index, int parent)
{
    project->RemoveTaskOrMilestone(index, parent);
}

void GanttController::SetFreeDays(Days days)
{
    this->freeDays->SetFreeDays(days);
}

void GanttController::SetFreeDaysColor(QColor color)
{
    this->freeDays->SetFreeDaysColor(color);
}

