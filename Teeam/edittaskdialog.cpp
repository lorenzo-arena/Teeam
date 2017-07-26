#include "edittaskdialog.h"
#include "ui_edittaskdialog.h"

EditTaskDialog::EditTaskDialog(TeeamProject *project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTaskDialog)
{

    ui->setupUi(this);

    // TODO : implementare!


    /*

    this->project = *project;

    // Setto i vari elementi della dialog
    ui->taskGroupcomboBox->addItem("None");
    ui->taskGroupcomboBox->setCurrentIndex(0);
    for (int i = 0; i < this->project.GetTaskGroup().length(); i++)
        ui->taskGroupcomboBox->addItem(this->project.GetTaskGroup().at(i)->getName());

    ui->baseGroupcomboBox->addItem("None");
    ui->baseGroupcomboBox->setCurrentIndex(0);
    for (int i = 0; i < this->project.GetTaskGroup().length(); i++)
        ui->baseGroupcomboBox->addItem(this->project.GetTaskGroup().at(i)->getName());

    ui->baseTaskcomboBox->addItem("None");
    ui->baseTaskcomboBox->setCurrentIndex(0);

    for (int i = 0; i < this->project.GetEntitiesList().length(); i++)
    {
        if(this->project.GetEntitiesList().at(i)->getEntityType() == TASK_CODE)
        {
            ui->baseTaskcomboBox->addItem(static_cast<Task*>(this->project.GetEntitiesList().at(i))->getName());
        }
    }

    ui->startdateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->enddateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(1));

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::Window);

    ui->lineEdit->installEventFilter(this);
    ui->startdateTimeEdit->installEventFilter(this);
    ui->enddateTimeEdit->installEventFilter(this);
    ui->completitionspinBox->installEventFilter(this);
    ui->lineEdit->setFocus();*/
}

EditTaskDialog::~EditTaskDialog()
{
    delete ui;
}



void EditTaskDialog::on_baseGroupcomboBox_currentIndexChanged(int index)
{
    if(index == 0 && ui->baseTaskcomboBox->currentIndex() == 0)
    {
        // in entrambe ho selezionato "None", disabilito tutti gli elemtni della dialog
        DisablePanel();
    }
}

void EditTaskDialog::on_baseTaskcomboBox_currentIndexChanged(int index)
{
    if(index == 0 && ui->baseGroupcomboBox->currentIndex() == 0)
    {
        // in entrambe ho selezionato "None", disabilito tutti gli elemtni della dialog
        DisablePanel();
    }
}

void EditTaskDialog::on_insertPeoplepushButton_clicked()
{

}

void EditTaskDialog::on_removePeoplepushButton_clicked()
{

}

void EditTaskDialog::on_taskGroupcomboBox_currentIndexChanged(int index)
{

}

void EditTaskDialog::on_buttonOk_clicked()
{

}

void EditTaskDialog::on_buttonCancel_clicked()
{

}

void EditTaskDialog::on_startdateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{

}

void EditTaskDialog::on_enddateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{

}

void EditTaskDialog::on_completitionspinBox_valueChanged(int arg1)
{

}

void EditTaskDialog::DisablePanel()
{
    ui->lineEdit->setEnabled(false);
    ui->taskGroupcomboBox->setEnabled(false);
    ui->totalPeoplelistView->setEnabled(false);
    ui->taskPeoplelistView->setEnabled(false);
    ui->insertPeoplepushButton->setEnabled(false);
    ui->removePeoplepushButton->setEnabled(false);
    ui->startdateTimeEdit->setEnabled(false);
    ui->enddateTimeEdit->setEnabled(false);
    ui->completitionspinBox->setEnabled(false);
}

void EditTaskDialog::EnablePanel()
{
    ui->lineEdit->setEnabled(true);
    ui->taskGroupcomboBox->setEnabled(true);
    ui->totalPeoplelistView->setEnabled(true);
    ui->taskPeoplelistView->setEnabled(true);
    ui->insertPeoplepushButton->setEnabled(true);
    ui->removePeoplepushButton->setEnabled(true);
    ui->startdateTimeEdit->setEnabled(true);
    ui->enddateTimeEdit->setEnabled(true);
    ui->completitionspinBox->setEnabled(true);
}

bool EditTaskDialog::eventFilter(QObject *target, QEvent *event)
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
