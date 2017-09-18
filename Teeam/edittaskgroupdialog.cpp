#include "edittaskgroupdialog.h"
#include "ui_edittaskgroupdialog.h"

#include <QMessageBox>

EditTaskGroupDialog::EditTaskGroupDialog(const QList<QString>& groups, QWidget *parent, int selectedGroup, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::EditTaskGroupDialog)
{
    ui->setupUi(this);

    this->existingGroups = groups;

    QDialog::setFixedSize(this->size());

    ui->lineEdit->installEventFilter(this);
    ui->namelistcomboBox->installEventFilter(this);
    ui->buttonOk->installEventFilter(this);
    ui->buttonCancel->installEventFilter(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::Window);

    ui->namelistcomboBox->addItem("None");
    ui->namelistcomboBox->setCurrentIndex(0);
    this->selectedGroup = 0;
    for (int i = 0; i < groups.length(); i++)
        ui->namelistcomboBox->addItem(groups.at(i));

    if(selectedGroup != -1)
    {
        this->selectedGroup = selectedGroup+1;
        ui->namelistcomboBox->setCurrentIndex(this->selectedGroup);
        ui->namelistcomboBox->setEnabled(false);
        ui->lineEdit->setText(existingGroups.at(this->selectedGroup-1));
    }

    ui->lineEdit->setFocus();
}

EditTaskGroupDialog::~EditTaskGroupDialog()
{
    delete ui;
}

QString EditTaskGroupDialog::GetTaskGroupName()
{
    return taskgroupName;
}

void EditTaskGroupDialog::on_buttonOk_clicked()
{
    if(selectedGroup == 0)
    {
        QDialog::reject();
        return;
    }

    QString editText = ui->lineEdit->text();
    if(editText == "")
    {
        QMessageBox::warning(this, "Error", "You must specify a name for the task group.", QMessageBox::Ok);
        return;
    }
    else
    {
        bool alreadyExists = false;
        for(int i = 0; i < existingGroups.length(); i++)
        {
            if(existingGroups.at(i) == ui->lineEdit->text() && i != (selectedGroup-1) )
                alreadyExists = true;
        }

        if(alreadyExists)
        {
            QMessageBox::warning(this, "Error", "You must specify a different name for every taskgroup.", QMessageBox::Ok);
            return;
        }

        taskgroupName = ui->lineEdit->text();
        QDialog::accept();
        return;
    }
}

void EditTaskGroupDialog::on_buttonCancel_clicked()
{
    QDialog::reject();
    return;
}

bool EditTaskGroupDialog::eventFilter(QObject* target, QEvent* event)
{
    if(target == ui->lineEdit && event->type() == QEvent::FocusIn)
    {
        ui->lineEdit->setCursorPosition(ui->lineEdit->text().length()); // Sposto il cursore in fondo alla stringa
        event->ignore();
    }
    else if (event->type() == QEvent::KeyPress)
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

void EditTaskGroupDialog::on_namelistcomboBox_currentIndexChanged(int index)
{
    selectedGroup = index;
    if(index == 0)
    {
        ui->lineEdit->setText("");
        ui->lineEdit->setEnabled(false);
    }
    else if(index > 0)
    {
        ui->lineEdit->setEnabled(true);
        ui->lineEdit->setText(existingGroups.at(index-1));
        ui->lineEdit->setFocus();
    }
}
