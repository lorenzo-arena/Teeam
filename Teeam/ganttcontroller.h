#ifndef GANTTCONTROLLER_H
#define GANTTCONTROLLER_H

#include "abstractview.h"
#include "freedaysmodel.h"
#include "taskgroup.h"
#include "task.h"
#include "milestone.h"
#include "teeamproject.h"

#include <QColor>

class GanttController
{
public:
    GanttController(FreeDaysModel *freeDays, TeeamProject *project = nullptr);

// controller interface
public:
    void NewProject(TeeamProject *newProject);
    void AddTaskGroup(AbstractView *view, QString taskGroupName);
    void AddTask(AbstractView *view, QString taskName, QDateTime start, QDateTime end, QList<QString> taskPeople, int completition, int selectedParent = -1);
    void AddMilestone(AbstractView *view, QString milestoneName, QDateTime start, QList<QString> taskPeople, int selectedParent = -1);
    void RemoveTaskGroup(int index);
    void RemoveTaskOrMilestone(int index, int parent = -1);
    void SetFreeDays(Days days);
    void SetFreeDaysColor(QColor color);

private:
    FreeDaysModel *freeDays;
    TeeamProject *project;
};

#endif // GANTTCONTROLLER_H
