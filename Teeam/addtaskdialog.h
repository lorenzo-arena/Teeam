#ifndef ADDTASKDIALOG_H
#define ADDTASKDIALOG_H

#include <QDialog>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QStringListModel>

namespace Ui {
class AddTaskDialog;
}

class AddTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTaskDialog(QList<QString> groupList, QList<QString> peopleList, QWidget *parent = 0);
    ~AddTaskDialog();
    int GetSelectedGroup() { return selectedGroup - 1; } // L'indice è zero-based
    QString GetTaskName() { return name; }
    QDateTime GetStartDateTime() { return start; }
    QDateTime GetEndDateTime() { return end; }
    QList<QString> GetPeople() { return taskPeopleModel.stringList(); }
    int GetCompletition() { return completition; }

private slots:
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();
    bool eventFilter(QObject *target, QEvent *event);
    void on_startdateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
    void on_enddateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
    void on_insertPeoplepushButton_clicked();
    void on_removePeoplepushButton_clicked();
    void on_taskGroupcomboBox_currentIndexChanged(int index);

    void on_completitionspinBox_valueChanged(int arg1);

private:
    Ui::AddTaskDialog *ui;
    QString name;
    int selectedGroup;
    QDateTime start;
    QDateTime end;
    QStringListModel totalPeopleModel;
    QStringListModel taskPeopleModel;
    int completition;

};

#endif // ADDTASKDIALOG_H
