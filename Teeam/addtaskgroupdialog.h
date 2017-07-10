#ifndef ADDTASKGROUPDIALOG_H
#define ADDTASKGROUPDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class AddTaskGroupDialog;
}

class AddTaskGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTaskGroupDialog(QWidget *parent = 0, Qt::WindowFlags f = 0 );

    QString GetProjectName();

private slots:
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();
    bool eventFilter(QObject *target, QEvent *event);

private:
    Ui::AddTaskGroupDialog* ui;
    QString taskgroupName;
};

#endif // ADDTASKGROUPDIALOG_H
