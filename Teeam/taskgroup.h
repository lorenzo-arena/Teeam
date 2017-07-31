#ifndef TASKGROUP_H
#define TASKGROUP_H

#include "ganttentity.h"
#include "generictask.h"
#include "teeamproject.h"

#include <QList>
#include <QColor>

class GenericTask;
class TeeamProject;

class TaskGroup : public GanttEntity
{
    friend class GenericTask;
    friend class TeeamProject;
public:
    TaskGroup(QString taskGroupName);
    QString getName();
    void setName(QString newName) { name = newName; }
    bool IsNew();
    void AddTask(GenericTask* task);
    QList<GenericTask*> GetEntitiesList() { return taskList; }
    bool IsGroupChanged() { return isGroupChanged; }

private:
    QDateTime end;
    QColor color;
    QList<GenericTask*> taskList;
    bool isNew;
    bool isGroupChanged; // utilizzato per notificare se ho un edit del group

protected:
    void setNew(bool b) { isNew = b; }
    void setRemoved(bool b) { isRemoved = b; }
    void setGroupChanged(bool b) { isGroupChanged = b; }
    void ReplaceEntity(int index, GenericTask* entity);

};

#endif // TASKGROUP_H
