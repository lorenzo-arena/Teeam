#ifndef TEEAMGANTTVIEW_H
#define TEEAMGANTTVIEW_H

#include "KDGanttView"
#include "abstractview.h"

class TeeamGanttView : public KDGantt::View, public AbstractView
{
public:
    TeeamGanttView(QWidget *parent);

    void UpdateView();
};

#endif // TEEAMGANTTVIEW_H
