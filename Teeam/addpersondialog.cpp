#include "addpersondialog.h"
#include "ui_addpersondialog.h"

#include <QMessageBox>

AddPersonDialog::AddPersonDialog(QStringList people, QWidget *parent, Qt::WindowFlags f ) :
    QDialog(parent, f),
    ui(new Ui::AddPersonDialog)
{
    ui->setupUi(this);

    QDialog::setFixedSize(this->size());

    ui->personNamelineEdit->installEventFilter(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::Window);
    setWindowFlags(windowFlags() | f);

    ui->personNamelineEdit->setFocus();

    this->actualPeople = people;
}

AddPersonDialog::~AddPersonDialog()
{
    delete ui;
}

bool AddPersonDialog::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return )
        {
            on_okbutton_clicked();
            return true;
        }
    }
    return QDialog::eventFilter(target, event);
}

void AddPersonDialog::on_okbutton_clicked()
{
    QString editText = ui->personNamelineEdit->text();
    if(editText == "")
    {
        QMessageBox::warning(this, "Error", "You must specify a name for the new person.", QMessageBox::Ok);
        return;
    }
    else
    {
        bool bAlreadyExist = false;
        for( int index = 0; index < actualPeople.length(); index++)
        {
            if(editText == actualPeople.at(index))
            {
                bAlreadyExist = true;
            }
        }

        if(bAlreadyExist)
        {
            QMessageBox::warning(this, "Error", "You must specify a different name for the new person.", QMessageBox::Ok);
            return;
        }

        personName = ui->personNamelineEdit->text();
        QDialog::accept();
    }
}

void AddPersonDialog::on_cancelbutton_clicked()
{
    this->close();
}
