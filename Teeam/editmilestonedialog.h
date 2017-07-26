#ifndef EDITMILESTONEDIALOG_H
#define EDITMILESTONEDIALOG_H

#include <QDialog>

namespace Ui {
class EditMilestoneDialog;
}

class EditMilestoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditMilestoneDialog(QWidget *parent = 0);
    ~EditMilestoneDialog();

private:
    Ui::EditMilestoneDialog *ui;
};

#endif // EDITMILESTONEDIALOG_H
