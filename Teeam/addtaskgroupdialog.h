#ifndef ADDTASKGROUPDIALOG_H
#define ADDTASKGROUPDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QKeyEvent>
#include <QList>
#include <QString>

namespace Ui {
class AddTaskGroupDialog;
}

class AddTaskGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTaskGroupDialog(QList<QString> existingGroups, QWidget *parent = 0, Qt::WindowFlags f = 0 );

    QString GetTaskGroupName();

private slots:
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();
    bool eventFilter(QObject *target, QEvent *event);

private:
    Ui::AddTaskGroupDialog* ui;
    QString taskgroupName;
    QList<QString> existingGroups;
};

#endif // ADDTASKGROUPDIALOG_H
