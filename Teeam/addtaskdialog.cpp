#include "addtaskdialog.h"
#include "ui_addtaskdialog.h"

#include <QDateTime>
#include <QMessageBox>

AddTaskDialog::AddTaskDialog(QList<QString> groupList, QList<QString> peopleList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTaskDialog)
{
    ui->setupUi(this);

    // Setto i vari elementi della dialog

    ui->taskGroupcomboBox->addItem("None");
    ui->taskGroupcomboBox->setCurrentIndex(0);
    selectedGroup = 0;
    for (int i = 0; i < groupList.length(); i++)
        ui->taskGroupcomboBox->addItem(groupList.at(i));

    QStringList totalPeopleList;

    for (int i = 0; i < peopleList.length(); i++)
        totalPeopleList << peopleList.at(i);

    totalPeopleModel.setStringList(totalPeopleList);
    ui->totalPeoplelistView->setModel(&totalPeopleModel);

    ui->startdateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->enddateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(1));

    start = ui->startdateTimeEdit->dateTime();
    end = ui->enddateTimeEdit->dateTime();

    completition = ui->completitionspinBox->value();

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::Window);

    ui->lineEdit->installEventFilter(this);
    ui->startdateTimeEdit->installEventFilter(this);
    ui->enddateTimeEdit->installEventFilter(this);
    ui->completitionspinBox->installEventFilter(this);
    ui->lineEdit->setFocus();
}

AddTaskDialog::~AddTaskDialog()
{
    delete ui;
}

void AddTaskDialog::on_buttonOk_clicked()
{
    // Estraggo il nome del task per controllarlo
    QString editText = ui->lineEdit->text();

    // Estraggo le date per controllarle
    QDateTime tempStart = ui->startdateTimeEdit->dateTime();
    QDateTime tempEnd = ui->enddateTimeEdit->dateTime();

    if(editText == "")
    {
        QMessageBox::warning(this, "Error", "You must specify a name for the new task.", QMessageBox::Ok);
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

void AddTaskDialog::on_buttonCancel_clicked()
{
    this->close();
}

void AddTaskDialog::on_startdateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    start = dateTime;
    if(end < start)
    {
        ui->enddateTimeEdit->setDateTime(start);
        end = start;
    }
}

void AddTaskDialog::on_enddateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    end = dateTime;
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

void AddTaskDialog::on_insertPeoplepushButton_clicked()
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

void AddTaskDialog::on_removePeoplepushButton_clicked()
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

void AddTaskDialog::on_taskGroupcomboBox_currentIndexChanged(int index)
{
    selectedGroup = index;
}

void AddTaskDialog::on_completitionspinBox_valueChanged(int arg1)
{
    completition = arg1;
}
