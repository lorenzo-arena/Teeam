#include "freedaysmodel.h"

#include <QListIterator>

FreeDaysModel::FreeDaysModel()
{
    viewers = new QList<AbstractView*>;

    days.bMonday = false;
    days.bTuesday = false;
    days.bWednesday = false;
    days.bThursday = false;
    days.bFriday = false;
    days.bSaturday = false;
    days.bSunday = false;

    bChanged = false;

    color.setNamedColor("white");
}

FreeDaysModel::~FreeDaysModel()
{
    delete viewers;
}

void FreeDaysModel::attach(AbstractView *view)
{
    viewers->append(view);
}

void FreeDaysModel::detach(AbstractView *view)
{
    viewers->removeAll(view);
}

void FreeDaysModel::notify()
{
    if(bChanged)
    {
        for(int i = 0; i < viewers->count(); i++)
            viewers->at(i)->UpdateView();
    }
    bChanged = false;
}

Days FreeDaysModel::GetFreeDays()
{
    return days;
}

void FreeDaysModel::SetFreeDays(Days days)
{
    this->days = days;
    bChanged = true;
    notify();
}

QColor FreeDaysModel::GetFreeDaysColor()
{
    return color;
}

void FreeDaysModel::SetFreeDaysColor(QColor color)
{
    this->color = color;
    bChanged = true;
    notify();
}

bool FreeDaysModel::GetChanged()
{
    return bChanged;
}


