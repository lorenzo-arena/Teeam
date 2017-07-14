#ifndef MILESTONE_H
#define MILESTONE_H

#include "generictask.h"

class Milestone : public GenericTask
{
public:
    Milestone();

public:
    void AddDependency(GenericTask* dependency);
};

#endif // MILESTONE_H
