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
    QDateTime end;
    QList<TaskGroup*> taskGroupList;

public:
    void AddTaskGroup(TaskGroup* taskGroup);
    QString getName();
    void setName(QString projectName);
};

#endif // TEEAMPROJECT_H
