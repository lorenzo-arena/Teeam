#ifndef TASK_H
#define TASK_H

#include "generictask.h"

class Task : public GenericTask
{
public:
    Task();

private:
    QDateTime end;

public:
    void AddDependency(GenericTask* dependency);
};

#endif // TASK_H
