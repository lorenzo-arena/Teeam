#ifndef GENERICTASK_H
#define GENERICTASK_H

#include <QColor>
#include <QList>

class GenericTask
{
public:
    GenericTask(){}

protected:
    QColor color;
    QList<GenericTask*> dependecies;

public:
    virtual void AddDependency(GenericTask* dependency) = 0;
};

#endif // GENERICTASK_H
