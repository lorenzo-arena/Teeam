#ifndef TEEAMGANTTVIEW_H
#define TEEAMGANTTVIEW_H

#include "abstractview.h"
#include "teeamproject.h"

#include <QList>

#include <KDGanttView>

class TeeamGanttView : public KDGantt::View, public AbstractView
{
public:
    TeeamGanttView(QWidget *parent);

public:
    void UpdateView();

private:
    QList<TeeamProject*> projects;
};

#endif // TEEAMGANTTVIEW_H
