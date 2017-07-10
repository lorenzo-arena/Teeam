#include "freedaysmodel.h"

FreeDaysModel::FreeDaysModel()
{
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
}

Days FreeDaysModel::GetFreeDays()
{
    return days;
}

void FreeDaysModel::SetFreeDays(Days days)
{
    if( this->days.bMonday != days.bMonday       ||
        this->days.bTuesday != days.bTuesday     ||
        this->days.bWednesday != days.bWednesday ||
        this->days.bThursday != days.bThursday   ||
        this->days.bFriday != days.bFriday       ||
        this->days.bSaturday != days.bSaturday   ||
        this->days.bSunday != days.bSunday )
    {
        bChanged = true;
        this->days = days;
    }

    if(bChanged)
        notify();

    bChanged = false;
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
    bChanged = false;
}


