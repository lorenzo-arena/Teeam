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
    QString getName() { return name; }
    QDateTime getStart() { return start; }
    QDateTime getEnd() { return end; }
    QList<QString> getPeople() { return people; }
    int getCompletition() { return completition; }
};

#endif // TASK_H
