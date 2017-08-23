#ifndef EDITTASKGROUPDIALOG_H
#define EDITTASKGROUPDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QKeyEvent>
#include <QList>
#include <QString>

namespace Ui {
class EditTaskGroupDialog;
}

class EditTaskGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTaskGroupDialog(const QList<QString>& groups, QWidget *parent = 0, int selectedGroup = -1, Qt::WindowFlags f = 0 );
    ~EditTaskGroupDialog();
    QString GetTaskGroupName();
    int GetSelectedGroup() { return selectedGroup - 1; } // Questo perché l'indice del gruppo nella lista è 0-based, mentre nella combo c'è la voce aggiuntiva "None"

private slots:
    bool eventFilter(QObject *target, QEvent *event);
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();
    void on_namelistcomboBox_currentIndexChanged(int index);

private:
    Ui::EditTaskGroupDialog* ui;
    QString taskgroupName;
    QList<QString> existingGroups;
    int selectedGroup;
};

#endif // EDITTASKGROUPDIALOG_H
