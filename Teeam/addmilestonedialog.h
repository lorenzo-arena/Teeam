#ifndef ADDMILESTONEDIALOG_H
#define ADDMILESTONEDIALOG_H

#include <QDialog>

namespace Ui {
class AddMilestoneDialog;
}

class AddMilestoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMilestoneDialog(QWidget *parent = 0);
    ~AddMilestoneDialog();

private:
    Ui::AddMilestoneDialog *ui;
};

#endif // ADDMILESTONEDIALOG_H
