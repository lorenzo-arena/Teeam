#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include "abstractview.h"

class AbstractModel
{
public:
    AbstractModel(){}
    virtual ~AbstractModel(){}

public:
    virtual void AbstractModel() = 0;
    virtual void attach(AbstractView *view) = 0;
    virtual void detach(AbstractView *view) = 0;
    virtual void notify() = 0;
};

#endif // ABSTRACTMODEL_H
