#ifndef FREEDAYSMODEL_H
#define FREEDAYSMODEL_H

#include "abstractmodel.h"
#include "abstractview.h"

#include <QList>
#include <QColor>

struct Days
{
    bool bMonday;
    bool bTuesday;
    bool bWednesday;
    bool bThursday;
    bool bFriday;
    bool bSaturday;
    bool bSunday;
};

class FreeDaysModel : public AbstractModel
{
public:
    FreeDaysModel();
    ~FreeDaysModel();

public:
    Days GetFreeDays();
    QColor GetFreeDaysColor();

    void SetFreeDays(Days days);
    void SetFreeDaysColor(QColor color);

private:
    Days days;
    QColor color;
};

#endif // FREEDAYSMODEL_H
