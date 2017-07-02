#include "addprojectdialog.h"
#include "ui_addprojectdialog.h"

#include <QMessageBox>

AddProjectDialog::AddProjectDialog(QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::AddProjectDialog)
{
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
    ui->label->installEventFilter(this);
    ui->buttonOk->installEventFilter(this);
    ui->buttonCancel->installEventFilter(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

QString AddProjectDialog::GetProjectName()
{
    return projectName;
}

void AddProjectDialog::on_buttonOk_clicked()
{
    QString editText = ui->lineEdit->text();
    if(editText == "")
    {
        QMessageBox::warning(this, "Error", "You must specify a name for the new project.", QMessageBox::Ok);
        return;
    }
    else
    {
        projectName = ui->lineEdit->text();
        this->close();
    }
}

void AddProjectDialog::on_buttonCancel_clicked()
{
    this->close();
}

bool AddProjectDialog::eventFilter(QObject* target, QEvent* event)
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
