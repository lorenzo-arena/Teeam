#ifndef GANTTCONTROLLER_H
#define GANTTCONTROLLER_H

#include "abstractview.h"
#include "freedaysmodel.h"
#include "taskgroup.h"
#include "task.h"
#include "milestone.h"
#include "teeamproject.h"

#include <QColor>
#include <QList>

class GanttController
{
public:
    GanttController(FreeDaysModel *freeDays, TeeamProject *project = nullptr);

    // controller interface
public:
    void NewProject(TeeamProject *newProject);
    void EditProject(QString newName, QList<QString> newPeople);
    void AddTaskGroup(AbstractView *view, QString taskGroupName);
    void EditTaskGroup(int index, QString newName);
    void EditTaskOrMilestone(AbstractView *view, QString entityName, QDateTime start, QDateTime end, QList<QString> taskPeople, int completition, int selectedParent, int index, int parent = -1);
    void EditTaskOrMilestone(AbstractView *view, QString entityName, QDateTime start, QList<QString> milestonePeople, int selectedParent, int index, int parent = -1);
    void AddTask(AbstractView *view, QString taskName, QDateTime start, QDateTime end, QList<QString> taskPeople, int completition, int selectedParent = -1);
    void AddMilestone(AbstractView *view, QString milestoneName, QDateTime start, QList<QString> taskPeople, int selectedParent = -1);
    void RemoveTaskGroup(int index);
    void RemoveTaskOrMilestone(int index, int parent = -1);
    void SetFreeDays(Days days);
    void SetFreeDaysColor(QColor color);
    void SaveProjectAs(const QString filename);
    int OpenFile(const QString filename, AbstractView *view);

private:
    FreeDaysModel *freeDays;
    TeeamProject *project;
};

#endif // GANTTCONTROLLER_H
