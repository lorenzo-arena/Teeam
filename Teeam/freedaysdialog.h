#ifndef FREEDAYSDIALOG_H
#define FREEDAYSDIALOG_H

#include <QDialog>

namespace Ui {
class FreeDaysDialog;
}

class FreeDaysDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FreeDaysDialog(QWidget *parent = 0);
    ~FreeDaysDialog();

private:
    Ui::FreeDaysDialog *ui;
};

#endif // FREEDAYSDIALOG_H
