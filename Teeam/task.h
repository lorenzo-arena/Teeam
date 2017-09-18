#ifndef TASK_H
#define TASK_H

#include "generictask.h"

#include <QList>
#include <QDateTime>

class Task : public GenericTask
{
public:
    Task(QString taskName, QDateTime start, QDateTime end, QList<QString> taskPeople, int completition);
    void edit(QString taskName, QDateTime start, QDateTime end, QList<QString> taskPeople, int completition);

private:
    QDateTime end;
    int completition;

public:
    void AddDependency(GenericTask* dependency);
    QString GetName() { return name; }
    QDateTime GetStart() { return start; }
    QDateTime GetEnd() { return end; }
    QList<QString> GetPeople() { return people; }
    int GetCompletition() { return completition; }
};

#endif // TASK_H
