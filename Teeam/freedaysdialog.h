#ifndef FREEDAYSDIALOG_H
#define FREEDAYSDIALOG_H

#include "freedaysmodel.h"

#include <QDialog>
#include <QColor>

namespace Ui {
class FreeDaysDialog;
}

class FreeDaysDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FreeDaysDialog(Days days, QColor color, QWidget *parent = 0);
    ~FreeDaysDialog();
    Days GetSelectedDays();
    QColor GetSelectedColor();

private slots:
    void on_buttonOK_clicked();
    void on_buttonCancel_clicked();
    void on_ButtonColor_clicked();

private:
    Ui::FreeDaysDialog *ui;
    Days days;
    QColor color;
};

#endif // FREEDAYSDIALOG_H
