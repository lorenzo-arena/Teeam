#ifndef GANNTENTITY_H
#define GANNTENTITY_H

#include "abstractmodel.h"

#include <QString>
#include <QDateTime>
#include <QList>

class GanttEntity : public AbstractModel
{
public:
    GanttEntity(){}
    virtual ~GanttEntity(){}

protected:
    QString name;
    QDateTime start;
    QList<QString> people;
};

#endif // GANNTENTITY_H
