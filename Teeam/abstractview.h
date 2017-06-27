#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H


class AbstractView
{
public:
    AbstractView(){}
    virtual ~AbstractView(){}

public:
    virtual void UpdateView() = 0;
};

#endif // ABSTRACTVIEW_H
