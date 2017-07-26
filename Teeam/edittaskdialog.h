#ifndef EDITTASKDIALOG_H
#define EDITTASKDIALOG_H

#include "teeamproject.h"
#include "ganttentity.h"
#include "generictask.h"
#include "taskgroup.h"
#include "task.h"

#include <QDialog>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QStringListModel>

namespace Ui {
class EditTaskDialog;
}

class EditTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTaskDialog(TeeamProject *project, QWidget *parent = 0);
    ~EditTaskDialog();
    int GetSelectedGroup() { return selectedGroup; }
    QString GetTaskName() { return name; }
    QDateTime GetStartDateTime() { return start; }
    QDateTime GetEndDateTime() { return end; }
    QList<QString> GetPeople() { return taskPeopleModel.stringList(); }
    int GetCompletition() { return completition; }
    //TeeamProject GetEdited() { return project; }

private slots:
    bool eventFilter(QObject *target, QEvent *event);
    void on_baseGroupcomboBox_currentIndexChanged(int index);
    void on_baseTaskcomboBox_currentIndexChanged(int index);
    void on_insertPeoplepushButton_clicked();
    void on_removePeoplepushButton_clicked();
    void on_taskGroupcomboBox_currentIndexChanged(int index);
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();
    void on_startdateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
    void on_enddateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
    void on_completitionspinBox_valueChanged(int arg1);

private:
    Ui::EditTaskDialog *ui;
    QString name;
    int selectedGroup;
    QDateTime start;
    QDateTime end;
    QStringListModel totalPeopleModel;
    QStringListModel taskPeopleModel;
    int completition;
    //TeeamProject project; // Inserire costruttore copia!
    void DisablePanel();
    void EnablePanel();
};

#endif // EDITTASKDIALOG_H
