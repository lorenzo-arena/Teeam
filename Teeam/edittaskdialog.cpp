#include "edittaskdialog.h"
#include "ui_edittaskdialog.h"

#include <QDateTime>
#include <QMessageBox>

EditTaskDialog::EditTaskDialog(TeeamProject *project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTaskDialog)
{
    ui->setupUi(this);

    singleEditMode = false;
    multipleEditMode = true;

    // TODO : implementare!

    /*
    this->project = *project;

    // Setto i vari elementi della dialog
    ui->taskGroupcomboBox->addItem("None");
    ui->taskGroupcomboBox->setCurrentIndex(0);
    for (int i = 0; i < this->project.GetTaskGroupListSize(); i++)
        ui->taskGroupcomboBox->addItem(this->project.GetTaskGroupAt(i)->GetName());

    ui->baseGroupcomboBox->addItem("None");
    ui->baseGroupcomboBox->setCurrentIndex(0);
    for (int i = 0; i < this->project.GetTaskGroupListSize(); i++)
        ui->baseGroupcomboBox->addItem(this->project.GetTaskGroupAt(i)->GetName());

    ui->baseTaskcomboBox->addItem("None");
    ui->baseTaskcomboBox->setCurrentIndex(0);

    for (int i = 0; i < this->project.GetEntitiesListSize(); i++)
    {
        if(this->project.GetEntityAt(i)->getEntityType() == TASK_CODE)
        {
            ui->baseTaskcomboBox->addItem(static_cast<Task*>(this->project.GetEntityAt(i))->GetName());
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

EditTaskDialog::EditTaskDialog(QString taskName, int group, QList<QString> taskGroups, QList<QString> taskPeople, QList<QString> totalPeople, QDateTime start, QDateTime end, int completition, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTaskDialog)
{
    ui->setupUi(this);

    singleEditMode = true;
    multipleEditMode = false;

    // Salvo i parametri
    this->name = taskName;
    this->selectedGroup = group;
    this->start = start;
    this->end = end;
    this->completition = completition;

    // Setto i vari elementi della dialog
    if(group >= 0)
        ui->baseGroupcomboBox->addItem(taskGroups.at(this->selectedGroup));
    else
        ui->baseGroupcomboBox->addItem("None");

    ui->baseGroupcomboBox->setCurrentIndex(0);
    ui->baseGroupcomboBox->setEnabled(false);

    ui->baseTaskcomboBox->addItem(taskName);
    ui->baseTaskcomboBox->setCurrentIndex(0);
    ui->baseTaskcomboBox->setEnabled(false);

    ui->lineEdit->setText(taskName);

    ui->taskGroupcomboBox->addItem("None");

    for (int i = 0; i < taskGroups.length(); i++)
        ui->taskGroupcomboBox->addItem(taskGroups.at(i));

    if(group >= 0)
        ui->taskGroupcomboBox->setCurrentIndex(group + 1);
    else
        ui->taskGroupcomboBox->setCurrentIndex(0);
    ui->taskGroupcomboBox->setEnabled(true);

    ui->startdateTimeEdit->setDateTime(start);
    ui->enddateTimeEdit->setDateTime(end);
    ui->completitionspinBox->setValue(completition);


    // Filtro la lista di persone in modo da aggiungere soltanto le persone giuste
    QStringList tempPeople;
    for(int i = 0; i < totalPeople.length(); i++)
    {
        if(!taskPeople.contains(totalPeople.at(i)))
            tempPeople << totalPeople.at(i);
    }

    totalPeopleModel.setStringList(tempPeople);
    ui->totalPeoplelistView->setModel(&totalPeopleModel);
    taskPeopleModel.setStringList(taskPeople);
    ui->taskPeoplelistView->setModel(&taskPeopleModel);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::Window);

    ui->lineEdit->installEventFilter(this);
    ui->startdateTimeEdit->installEventFilter(this);
    ui->taskGroupcomboBox->installEventFilter(this);
    ui->enddateTimeEdit->installEventFilter(this);
    ui->completitionspinBox->installEventFilter(this);
    ui->lineEdit->setFocus();
}

EditTaskDialog::~EditTaskDialog()
{
    delete ui;
}

void EditTaskDialog::on_baseGroupcomboBox_currentIndexChanged(int index)
{
    if(index == 0 && ui->baseTaskcomboBox->currentIndex() == 0 && multipleEditMode)
    {
        // in entrambe ho selezionato "None", disabilito tutti gli elemtni della dialog
        disablePanel();
    }
}

void EditTaskDialog::on_baseTaskcomboBox_currentIndexChanged(int index)
{
    if(index == 0 && ui->baseGroupcomboBox->currentIndex() == 0 && multipleEditMode)
    {
        // in entrambe ho selezionato "None", disabilito tutti gli elemtni della dialog
        disablePanel();
    }
}

void EditTaskDialog::on_insertPeoplepushButton_clicked()
{
    QModelIndex selection = ui->totalPeoplelistView->currentIndex();
    if(selection.isValid())
    {
        // Tiro su il nome della persona
        QString person = ui->totalPeoplelistView->model()->data(selection).toString();
        // Lo rimuovo dalla prima lista
        totalPeopleModel.removeRow(selection.row());
        // Lo aggiungo alla seconda
        QStringList tempList = taskPeopleModel.stringList();
        tempList << person;
        taskPeopleModel.setStringList(tempList);
        ui->taskPeoplelistView->setModel(&taskPeopleModel);
    }
}

void EditTaskDialog::on_removePeoplepushButton_clicked()
{
    QModelIndex selection = ui->taskPeoplelistView->currentIndex();
    if(selection.isValid())
    {
        // Tiro su il nome della persona
        QString person = ui->taskPeoplelistView->model()->data(selection).toString();
        // Lo rimuovo dalla prima lista
        taskPeopleModel.removeRow(selection.row());
        // Lo aggiungo alla seconda
        QStringList tempList = totalPeopleModel.stringList();
        tempList << person;
        totalPeopleModel.setStringList(tempList);
        ui->totalPeoplelistView->setModel(&totalPeopleModel);
    }
}

void EditTaskDialog::on_taskGroupcomboBox_currentIndexChanged(int index)
{
    selectedGroup = index;
}

void EditTaskDialog::on_buttonOk_clicked()
{
    // Estraggo il nome del task per controllarlo
    QString editText = ui->lineEdit->text();

    // Estraggo le date per controllarle
    QDateTime tempStart = ui->startdateTimeEdit->dateTime();
    QDateTime tempEnd = ui->enddateTimeEdit->dateTime();

    if(editText == "")
    {
        QMessageBox::warning(this, "Error", "You must specify a name for the task.", QMessageBox::Ok);
        return;
    }
    else if(tempEnd < tempStart)
    {
        QMessageBox::warning(this, "Error", "Start date must be before end date.", QMessageBox::Ok);
        return;
    }
    else
    {
        start = ui->startdateTimeEdit->dateTime();
        end = ui->enddateTimeEdit->dateTime();
        name = ui->lineEdit->text();
        QDialog::accept();
    }
}

void EditTaskDialog::on_buttonCancel_clicked()
{
    this->close();
}

void EditTaskDialog::on_startdateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    start = dateTime;
    if(end < start)
    {
        ui->enddateTimeEdit->setDateTime(start);
        end = start;
    }
}

void EditTaskDialog::on_enddateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    end = dateTime;
}

void EditTaskDialog::on_completitionspinBox_valueChanged(int arg1)
{
    completition = arg1;
}

void EditTaskDialog::disablePanel()
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

void EditTaskDialog::enablePanel()
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
