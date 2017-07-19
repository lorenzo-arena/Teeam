#include "addtaskgroupdialog.h"
#include "ui_addtaskgroupdialog.h"

#include <QMessageBox>

AddTaskGroupDialog::AddTaskGroupDialog(QList<QString> existingGroups, QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::AddTaskGroupDialog)
{
    ui->setupUi(this);

    QDialog::setFixedSize(this->size());

    ui->lineEdit->installEventFilter(this);
    ui->label->installEventFilter(this);
    ui->buttonOk->installEventFilter(this);
    ui->buttonCancel->installEventFilter(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::Window);

    ui->lineEdit->setFocus();

    this->existingGroups = existingGroups;
}

QString AddTaskGroupDialog::GetTaskGroupName()
{
    return taskgroupName;
}

void AddTaskGroupDialog::on_buttonOk_clicked()
{
    QString editText = ui->lineEdit->text();
    if(editText == "")
    {
        QMessageBox::warning(this, "Error", "You must specify a name for the new task group.", QMessageBox::Ok);
        return;
    }
    else
    {
        bool alreadyExists = false;
        for(int i = 0; i < existingGroups.length(); i++)
        {
            if(existingGroups.at(i) == ui->lineEdit->text())
                alreadyExists = true;
        }

        if(alreadyExists)
        {
            QMessageBox::warning(this, "Error", "You must specify a different name for every taskgroup.", QMessageBox::Ok);
            return;
        }

        taskgroupName = ui->lineEdit->text();
        QDialog::accept();
    }
}

void AddTaskGroupDialog::on_buttonCancel_clicked()
{
    this->close();
}

bool AddTaskGroupDialog::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return )
        {
            on_buttonOk_clicked();
            return true;
        }
    }
    return QDialog::eventFilter(target, event);
}
