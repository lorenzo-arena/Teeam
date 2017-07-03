#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include "observer.h"

class AbstractView : public Observer
{
public:
    AbstractView(){}
    virtual ~AbstractView(){}

public:
    virtual void UpdateView() = 0;
};

#endif // ABSTRACTVIEW_H
