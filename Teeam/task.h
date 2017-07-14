#ifndef TASK_H
#define TASK_H

#include "generictask.h"

#include <QList>
#include <QDateTime>

class Task : public GenericTask
{
public:
    Task(QString taskName, QDateTime start, QDateTime end, QList<QString> taskPeople);

private:
    QDateTime end;
    QList<QString> people;

public:
    void AddDependency(GenericTask* dependency);
    QString getName() { return name; }
    QDateTime getStart() { return start; }
    QDateTime getEnd() { return end; }
    QList<QString> getPeople() { return people; }

};

#endif // TASK_H
