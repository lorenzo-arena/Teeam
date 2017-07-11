#ifndef ADDTASKDIALOG_H
#define ADDTASKDIALOG_H

#include <QDialog>
#include <QString>
#include <QList>

namespace Ui {
class AddTaskDialog;
}

class AddTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTaskDialog(QList<QString> groupList, QWidget *parent = 0);
    ~AddTaskDialog();
    QString GetSelectedGroup();
    QString GetTaskName();

private slots:
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();
    bool eventFilter(QObject *target, QEvent *event);

    void on_taskGroupcomboBox_currentTextChanged(const QString &arg1);

private:
    Ui::AddTaskDialog *ui;
    QString taskName;
    QString selectedGroup;
};

#endif // ADDTASKDIALOG_H
