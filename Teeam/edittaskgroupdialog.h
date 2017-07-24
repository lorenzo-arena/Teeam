#ifndef EDITTASKGROUPDIALOG_H
#define EDITTASKGROUPDIALOG_H

#include <QDialog>

namespace Ui {
class EditTaskGroupDialog;
}

class EditTaskGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTaskGroupDialog(QWidget *parent = 0);
    ~EditTaskGroupDialog();

private:
    Ui::EditTaskGroupDialog *ui;
};

#endif // EDITTASKGROUPDIALOG_H
