#ifndef GANNTENTITY_H
#define GANNTENTITY_H

#include "abstractmodel.h"

#include <QString>
#include <QDateTime>
#include <QList>

class GanttEntity : public AbstractModel
{
public:
    GanttEntity(){ isNew = true; isRemoved = false; }
    virtual ~GanttEntity(){}
    bool IsRemoved() { return isRemoved; }

protected:
    QString name;
    QDateTime start;
    QList<QString> people;
    bool isNew;
    bool isRemoved;
};

#endif // GANNTENTITY_H
