#ifndef ADDTASKDIALOG_H
#define ADDTASKDIALOG_H

#include <QDialog>
#include <QString>
#include <QList>
#include <QDateTime>

namespace Ui {
class AddTaskDialog;
}

class AddTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTaskDialog(QList<QString> groupList, QWidget *parent = 0);
    ~AddTaskDialog();
    QString GetSelectedGroup() { return selectedGroup; }
    QString GetTaskName() { return name; }
    QDateTime GetStartDateTime() { return start; }
    QDateTime GetEndDateTime() { return end; }

private slots:
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();
    bool eventFilter(QObject *target, QEvent *event);

    void on_taskGroupcomboBox_currentTextChanged(const QString &arg1);

    void on_startdateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_enddateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

private:
    Ui::AddTaskDialog *ui;
    QString name;
    QString selectedGroup;
    QDateTime start;
    QDateTime end;
};

#endif // ADDTASKDIALOG_H
