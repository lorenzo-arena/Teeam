#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include "abstractview.h"

#include <QList>

class AbstractModel
{
public:
    AbstractModel(){ changed = false; }
    virtual ~AbstractModel(){}

public:
    void attach(AbstractView *view) { viewers.append(view); }
    void detach(AbstractView *view) { viewers.removeAll(view); }
    void notify() {
        if(changed)
        {
            for(int i = 0; i < viewers.count(); i++)
                viewers.at(i)->UpdateView();
        }
        changed = false;
    }
    bool isChanged() { return changed; }

protected:
    bool changed;
    void setChanged(bool b) { changed = b; }

    QList<AbstractView *> viewers;
};

#endif // ABSTRACTMODEL_H
