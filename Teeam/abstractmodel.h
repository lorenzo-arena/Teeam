#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include "abstractview.h"

#include <QList>

class AbstractModel
{
public:
    AbstractModel(){ bChanged = false; }
    virtual ~AbstractModel(){}

public:
    void attach(AbstractView *view) { viewers.append(view); }
    void detach(AbstractView *view) { viewers.removeAll(view); }
    void notify() {
        if(bChanged)
        {
            for(int i = 0; i < viewers.count(); i++)
                viewers.at(i)->UpdateView();
        }
        bChanged = false;
    }
    bool isChanged() { return bChanged; }

protected:
    bool bChanged;
    void setChanged(bool b) { bChanged = b; }

    QList<AbstractView *> viewers;
};

#endif // ABSTRACTMODEL_H
