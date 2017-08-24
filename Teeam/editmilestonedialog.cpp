#include "editmilestonedialog.h"
#include "ui_editmilestonedialog.h"

#include <QDateTime>
#include <QMessageBox>

EditMilestoneDialog::EditMilestoneDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditMilestoneDialog)
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

EditMilestoneDialog::EditMilestoneDialog(QString milestoneName, int group, QList<QString> milestoneGroups, QList<QString> milestonePeople, QList<QString> totalPeople, QDateTime start, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditMilestoneDialog)
{
    ui->setupUi(this);

    singleEditMode = true;
    multipleEditMode = false;

    // Salvo i parametri
    this->name = milestoneName;
    this->selectedGroup = group;
    this->start = start;

    // Setto i vari elementi della dialog
    if(group >= 0)
        ui->baseGroupcomboBox->addItem(milestoneGroups.at(this->selectedGroup));
    else
        ui->baseGroupcomboBox->addItem("None");
    ui->baseGroupcomboBox->setCurrentIndex(0);
    ui->baseGroupcomboBox->setEnabled(false);

    ui->baseMilestonecomboBox->addItem(milestoneName);
    ui->baseMilestonecomboBox->setCurrentIndex(0);
    ui->baseMilestonecomboBox->setEnabled(false);

    ui->lineEdit->setText(milestoneName);

    ui->milestoneGroupcomboBox->addItem("None");
    for (int i = 0; i < milestoneGroups.length(); i++)
        ui->milestoneGroupcomboBox->addItem(milestoneGroups.at(i));
    if(group >= 0)
        ui->milestoneGroupcomboBox->setCurrentIndex(group + 1);
    else
        ui->milestoneGroupcomboBox->setCurrentIndex(0);
    ui->milestoneGroupcomboBox->setEnabled(true);

    ui->startdateTimeEdit->setDateTime(start);

    // Filtro la lista di persone in modo da aggiungere soltanto le persone giuste
    QStringList tempPeople;
    for(int i = 0; i < totalPeople.length(); i++)
    {
        if(!milestonePeople.contains(totalPeople.at(i)))
            tempPeople << totalPeople.at(i);
    }

    totalPeopleModel.setStringList(tempPeople);
    ui->totalPeoplelistView->setModel(&totalPeopleModel);
    milestonePeopleModel.setStringList(milestonePeople);
    ui->milestonePeoplelistView->setModel(&milestonePeopleModel);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::Window);

    ui->lineEdit->installEventFilter(this);
    ui->startdateTimeEdit->installEventFilter(this);
    ui->lineEdit->setFocus();
}

EditMilestoneDialog::~EditMilestoneDialog()
{
    delete ui;
}

void EditMilestoneDialog::on_baseGroupcomboBox_currentIndexChanged(int index)
{
    if(index == 0 && ui->baseMilestonecomboBox->currentIndex() == 0 && multipleEditMode)
    {
        // in entrambe ho selezionato "None", disabilito tutti gli elemtni della dialog
        DisablePanel();
    }
}

void EditMilestoneDialog::on_baseMilestonecomboBox_currentIndexChanged(int index)
{
    if(index == 0 && ui->baseGroupcomboBox->currentIndex() == 0 && multipleEditMode)
    {
        // in entrambe ho selezionato "None", disabilito tutti gli elemtni della dialog
        DisablePanel();
    }
}

void EditMilestoneDialog::on_buttonOk_clicked()
{
    // Estraggo il nome della milestone per controllarlo
    QString editText = ui->lineEdit->text();

    if(editText == "")
    {
        QMessageBox::warning(this, "Error", "You must specify a name for the milestone.", QMessageBox::Ok);
        return;
    }
    else
    {
        start = ui->startdateTimeEdit->dateTime();
        name = ui->lineEdit->text();
        QDialog::accept();
    }
}

void EditMilestoneDialog::on_buttonCancel_clicked()
{
    this->close();
}

void EditMilestoneDialog::on_insertPeoplepushButton_clicked()
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

void EditMilestoneDialog::on_removePeoplepushButton_clicked()
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

void EditMilestoneDialog::on_startdateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    start = dateTime;
}

void EditMilestoneDialog::on_milestoneGroupcomboBox_currentIndexChanged(int index)
{
    selectedGroup = index;
}

void EditMilestoneDialog::DisablePanel()
{
    ui->lineEdit->setEnabled(false);
    ui->milestoneGroupcomboBox->setEnabled(false);
    ui->totalPeoplelistView->setEnabled(false);
    ui->milestonePeoplelistView->setEnabled(false);
    ui->insertPeoplepushButton->setEnabled(false);
    ui->removePeoplepushButton->setEnabled(false);
    ui->startdateTimeEdit->setEnabled(false);
}

void EditMilestoneDialog::EnablePanel()
{
    ui->lineEdit->setEnabled(true);
    ui->milestoneGroupcomboBox->setEnabled(true);
    ui->totalPeoplelistView->setEnabled(true);
    ui->milestonePeoplelistView->setEnabled(true);
    ui->insertPeoplepushButton->setEnabled(true);
    ui->removePeoplepushButton->setEnabled(true);
    ui->startdateTimeEdit->setEnabled(true);
}

bool EditMilestoneDialog::eventFilter(QObject *target, QEvent *event)
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
