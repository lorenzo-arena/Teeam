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
    explicit TaskGroup(QString taskGroupName);
    ~TaskGroup();
    QString GetName();
    void setName(QString newName) { name = newName; }
    bool IsNew();
    void AddTask(GenericTask* task);
    bool IsGroupChanged() { return isGroupChanged; }
    GenericTask* GetEntityAt(int index) { return taskList.at(index); }
    int GetEntitiesListSize() { return taskList.length(); }

private:
    QDateTime end;
    QColor color;
    QList<GenericTask*> taskList;
    bool isGroupChanged; // utilizzato per notificare se ho un edit del group

protected:
    void setNew(bool b) { isNew = b; }
    void setRemoved(bool b) { isRemoved = b; }
    void setGroupChanged(bool b) { isGroupChanged = b; }
    void replaceEntity(int index, GenericTask* entity);
    void removeEntityAt(int index);
    void setBeforeCreation();
    void resetAfterCreation();
    void searchAndRemovePerson(QString person);
};

#endif // TASKGROUP_H
