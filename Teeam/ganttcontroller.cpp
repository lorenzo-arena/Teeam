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
    if(index < project->GetTaskGroup().length())
    {
        project->GetTaskGroup().at(index)->setName(newName);
        project->ShowGroups();
    }
}

void GanttController::EditTaskOrMilestone(AbstractView *view, QString entityName, QDateTime start, QDateTime end, QList<QString> taskPeople, int completition, int selectedParent, int index, int parent)
{
    if(selectedParent == parent)
    {
        Task *task = new Task(entityName, start, end, taskPeople, completition);
        task->attach(view);

        if(parent == -1)
        {
            // è un task che non appartiene a nessun gruppo
            project->EditTaskOrMilestone(task, index);
        }
        else
        {
            // è un task che appartiene a un gruppo
            project->EditTaskOrMilestone(task, index, parent);
        }
    }
    else
    {
        // entro qui se ho cambiato il gruppo del task,
        // lo cancello e ne creo uno nuovo nel gruppo di destinazione
        RemoveTaskOrMilestone(index, parent);
        AddTask(view, entityName, start, end, taskPeople, completition, selectedParent);
    }
}

void GanttController::EditTaskOrMilestone(AbstractView *view, QString entityName, QDateTime start, QList<QString> milestonePeople, int selectedParent, int index, int parent)
{
    if(selectedParent == parent)
    {
        Milestone *milestone = new Milestone(entityName, start, milestonePeople);
        milestone->attach(view);
        if(parent == -1)
        {
            // è una milestone che non appartiene a nessun gruppo
            project->EditTaskOrMilestone(milestone, index);
        }
        else
        {
            // è una milestone che appartiene a un gruppo
            project->EditTaskOrMilestone(milestone, index, parent);
        }
    }
    else
    {
        // entro qui se ho cambiato il gruppo della milestone,
        // la cancella e ne creo una nuova nel gruppo di destinazione
        RemoveTaskOrMilestone(index, parent);
        AddMilestone(view, entityName, start, milestonePeople, selectedParent);
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

