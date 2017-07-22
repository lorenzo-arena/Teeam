#include "addprojectdialog.h"
#include "ui_addprojectdialog.h"
#include "addpersondialog.h"

#include <QMessageBox>

AddProjectDialog::AddProjectDialog(QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::AddProjectDialog)
{
    // Costruttore utilizzato per creare un nuovo progetto
    ui->setupUi(this);

    QDialog::setFixedSize(this->size());

    ui->lineEdit->installEventFilter(this);
    ui->label->installEventFilter(this);
    ui->buttonOk->installEventFilter(this);
    ui->buttonCancel->installEventFilter(this);
    ui->removePersonButton->installEventFilter(this);
    ui->addPersonButton->installEventFilter(this);
    ui->peoplelistView->installEventFilter(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::Window);

    ui->peoplelistView->setModel(&peopleModel);

    ui->lineEdit->setFocus();
}

AddProjectDialog::AddProjectDialog(QString name, QList<QString> people, QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::AddProjectDialog)
{
    // Costruttore utilizzato per modificare il progetto esistente
    ui->setupUi(this);

    QDialog::setFixedSize(this->size());

    ui->lineEdit->installEventFilter(this);
    ui->label->installEventFilter(this);
    ui->buttonOk->installEventFilter(this);
    ui->buttonCancel->installEventFilter(this);
    ui->removePersonButton->installEventFilter(this);
    ui->addPersonButton->installEventFilter(this);
    ui->peoplelistView->installEventFilter(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::Window);

    ui->lineEdit->setText(name);
    peopleModel.setStringList(people);

    ui->peoplelistView->setModel(&peopleModel);
    ui->lineEdit->setFocus();
    ui->lineEdit->setCursorPosition(0);
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
        QDialog::accept();
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

void AddProjectDialog::on_addPersonButton_clicked()
{
    AddPersonDialog *dialog = new AddPersonDialog( this );
    if ( dialog->exec() == QDialog::Rejected || !dialog ) {
        delete dialog;
        return;
    }

    // Lo aggiungo alla seconda
    QStringList tempList = peopleModel.stringList();
    tempList << dialog->GetPersonName();
    peopleModel.setStringList(tempList);
    ui->peoplelistView->setModel(&peopleModel);

    delete dialog;
    return;
}

void AddProjectDialog::on_removePersonButton_clicked()
{
    QModelIndex selection = ui->peoplelistView->currentIndex();
    if(selection.isValid())
        peopleModel.removeRow(selection.row());
}
