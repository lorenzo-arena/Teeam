#ifndef MILESTONE_H
#define MILESTONE_H

#include "generictask.h"

class Milestone : public GenericTask
{
public:
    Milestone(QString milestoneName, QDateTime start, QList<QString> taskPeople);
    void edit(QString milestoneName, QDateTime start, QList<QString> taskPeople);
    QString getName() { return name; }
    QDateTime getDateTime() { return start; }
    QList<QString> getPeople() { return people; }

public:
    void AddDependency(GenericTask* dependency);
};

#endif // MILESTONE_H
