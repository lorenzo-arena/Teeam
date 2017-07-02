#ifndef GANNTENTITY_H
#define GANNTENTITY_H

#include "observer.h"

#include <QString>
#include <QDateTime>
#include <QList>

class GanttEntity
{
public:
    GanttEntity(){}
    virtual ~GanttEntity(){}

protected:
    QString name;
    QDateTime start;
    QDateTime end;
    QList<QString> people;
    QList<Observer*> observers;
};

#endif // GANNTENTITY_H
