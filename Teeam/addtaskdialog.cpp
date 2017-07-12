#include "addtaskdialog.h"
#include "ui_addtaskdialog.h"

#include <QDateTime>
#include <QMessageBox>

AddTaskDialog::AddTaskDialog(QList<QString> groupList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTaskDialog)
{
    ui->setupUi(this);

    // Setto i vari elementi della dialog
    ui->taskGroupcomboBox->addItem("None");
    ui->taskGroupcomboBox->setCurrentIndex(0);
    for (int i = 0; i < groupList.length(); i++)
        ui->taskGroupcomboBox->addItem(groupList.at(i));

    ui->startdateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->enddateTimeEdit->setDateTime(QDateTime::currentDateTime());

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::Window);

    ui->lineEdit->setFocus();
}

AddTaskDialog::~AddTaskDialog()
{
    delete ui;
}

QString AddTaskDialog::GetSelectedGroup()
{
    return selectedGroup;
}

QString AddTaskDialog::GetTaskName()
{
    return taskName;
}

void AddTaskDialog::on_buttonOk_clicked()
{
    QString editText = ui->lineEdit->text();
    if(editText == "")
    {
        QMessageBox::warning(this, "Error", "You must specify a name for the new project.", QMessageBox::Ok);
        return;
    }
    else
    {
        taskName = ui->lineEdit->text();
        QDialog::accept();
    }
}

void AddTaskDialog::on_buttonCancel_clicked()
{
    this->close();
}

bool AddTaskDialog::eventFilter(QObject* target, QEvent* event)
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

void AddTaskDialog::on_taskGroupcomboBox_currentTextChanged(const QString &arg1)
{
    selectedGroup = arg1;
}
