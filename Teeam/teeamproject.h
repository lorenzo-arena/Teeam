#ifndef TEEAMPROJECT_H
#define TEEAMPROJECT_H

#include "abstractmodel.h"
#include "abstractview.h"
#include "ganttentity.h"
#include "taskgroup.h"

#include <QList>

class TeeamProject: public GanttEntity, public AbstractModel
{
public:
    TeeamProject();

private:
    QList<TaskGroup*> taskGroupList;
    bool taskGroupChanged;
    bool projectChanged;

public:
    void AddTaskGroup(TaskGroup* taskGroup);
    QList<TaskGroup *> GetTaskGroup();
    bool isTaskGroupChanged();
    bool isProjectChanged();
    QString getName();
    void setName(QString projectName);
};

#endif // TEEAMPROJECT_H
