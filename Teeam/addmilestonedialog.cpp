#include "addmilestonedialog.h"
#include "ui_addmilestonedialog.h"

#include <QDateTime>
#include <QMessageBox>

AddMilestoneDialog::AddMilestoneDialog(QList<QString> groupList, QList<QString> peopleList, QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::AddMilestoneDialog)
{
    ui->setupUi(this);

    // Setto i vari elementi della dialog
    ui->milestoneGroupcomboBox->addItem("None");
    ui->milestoneGroupcomboBox->setCurrentIndex(0);
    for (int i = 0; i < groupList.length(); i++)
        ui->milestoneGroupcomboBox->addItem(groupList.at(i));

    QStringList totalPeopleList;

    for (int i = 0; i < peopleList.length(); i++)
        totalPeopleList << peopleList.at(i);

    totalPeopleModel.setStringList(totalPeopleList);
    ui->totalPeoplelistView->setModel(&totalPeopleModel);

    ui->startdateTimeEdit->setDateTime(QDateTime::currentDateTime());
    start = ui->startdateTimeEdit->dateTime();

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::Window);

    ui->lineEdit->installEventFilter(this);
    ui->startdateTimeEdit->installEventFilter(this);
    ui->milestoneGroupcomboBox->installEventFilter(this);
    ui->lineEdit->setFocus();
}

AddMilestoneDialog::~AddMilestoneDialog()
{
    delete ui;
}

void AddMilestoneDialog::on_buttonOk_clicked()
{
    // Estraggo il nome del milestone per controllarlo
    QString editText = ui->lineEdit->text();

    // Estraggo la data per controllarle
    QDateTime tempStart = ui->startdateTimeEdit->dateTime();

    if(editText == "")
    {
        QMessageBox::warning(this, "Error", "You must specify a name for the new milestone.", QMessageBox::Ok);
        return;
    }
    else
    {
        start = ui->startdateTimeEdit->dateTime();
        name = ui->lineEdit->text();
        QDialog::accept();
    }
}

void AddMilestoneDialog::on_buttonCancel_clicked()
{
    this->close();
}

void AddMilestoneDialog::on_startdateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    start = dateTime;
}

void AddMilestoneDialog::on_insertPeoplepushButton_clicked()
{
    QModelIndex selection = ui->totalPeoplelistView->currentIndex();
    if(selection.isValid())
    {
        // Tiro su il nome della persona
        QString person = ui->totalPeoplelistView->model()->data(selection).toString();
        // Lo rimuovo dalla prima lista
        totalPeopleModel.removeRow(selection.row());
        // Lo aggiungo alla seconda
        QStringList tempList = milestonePeopleModel.stringList();
        tempList << person;
        milestonePeopleModel.setStringList(tempList);
        ui->milestonePeoplelistView->setModel(&milestonePeopleModel);
    }
}

void AddMilestoneDialog::on_removePeoplepushButton_clicked()
{
    QModelIndex selection = ui->milestonePeoplelistView->currentIndex();
    if(selection.isValid())
    {
        // Tiro su il nome della persona
        QString person = ui->milestonePeoplelistView->model()->data(selection).toString();
        // Lo rimuovo dalla prima lista
        milestonePeopleModel.removeRow(selection.row());
        // Lo aggiungo alla seconda
        QStringList tempList = totalPeopleModel.stringList();
        tempList << person;
        totalPeopleModel.setStringList(tempList);
        ui->totalPeoplelistView->setModel(&totalPeopleModel);
    }
}

bool AddMilestoneDialog::eventFilter(QObject* target, QEvent* event)
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

void AddMilestoneDialog::on_milestoneGroupcomboBox_currentIndexChanged(int index)
{
    selectedGroup = index;
}
