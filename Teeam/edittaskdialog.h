#ifndef EDITTASKDIALOG_H
#define EDITTASKDIALOG_H

#include <QDialog>

namespace Ui {
class EditTaskDialog;
}

class EditTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTaskDialog(QWidget *parent = 0);
    ~EditTaskDialog();

private:
    Ui::EditTaskDialog *ui;
};

#endif // EDITTASKDIALOG_H
