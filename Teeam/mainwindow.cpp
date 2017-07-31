#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "addprojectdialog.h"
#include "addtaskgroupdialog.h"
#include "addtaskdialog.h"
#include "addmilestonedialog.h"
#include "edittaskgroupdialog.h"
#include "edittaskdialog.h"
#include "editmilestonedialog.h"
#include "freedaysdialog.h"

#include <QDebug>
#include <QApplication>
#include <QAbstractItemView>
#include <QList>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTreeView>
#include <QSettings>
#include <QEvent>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QSet>
#include <QLocale>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFileDialog>

TeeamDateTimeScaleFormatter::TeeamDateTimeScaleFormatter(const KDGantt::DateTimeScaleFormatter &other)
    : DateTimeScaleFormatter(other)
{

}

QString TeeamDateTimeScaleFormatter::text(const QDateTime &datetime)
{
    QString result;
    // additional feature: Weeknumber
    const QString shortWeekNumber = QString::number( datetime.date().weekNumber()) + QLatin1String("/")
                                                                                     + QString::number( datetime.date().year());
    const QString longWeekNumber = ( shortWeekNumber.length() == 1 ? QString::fromLatin1( "0" ) : QString() ) + shortWeekNumber;
    result.replace( QString::fromLatin1( "ww" ), longWeekNumber );
    result.replace( QString::fromLatin1( "w" ), shortWeekNumber );
    QLocale locale(QLocale::English, QLocale::UnitedStates);
    result = locale.toString(datetime, QLocale::ShortFormat);
    return result;
}

MainWindow::MainWindow(GanttController *ganttController, FreeDaysModel *freeDaysModel, TeeamProject *projectModel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Aggancio i vari modelli
    this->freeDaysModel = freeDaysModel;
    this->freeDaysModel->attach(this);

    this->projectModel = projectModel;
    if(projectModel != nullptr)
        this->projectModel->attach(this);

    this->ganttController = ganttController;

    costraintModel = new KDGantt::ConstraintModel(this);
    ui->ganttView->setConstraintModel(costraintModel);

    initGanttView();

    // Prelevo i settings dai registri
    QSettings settings;
    settings.beginGroup(REG_KEY_MAINWINDOW);
    if(settings.contains(REG_KEY_MAINWINDOW_MAXIMIZED))    // setto la dimensione della finestra
    {
        if(settings.value(REG_KEY_MAINWINDOW_MAXIMIZED).toBool())
            showMaximized();
    }
    else if (settings.contains(REG_KEY_MAINWINDOW_SIZE))
    {
        resize(settings.value(REG_KEY_MAINWINDOW_SIZE).toSize());
    }
    settings.endGroup();

    settings.beginGroup(REG_KEY_DATETIMEVIEW);
    // lancio un update dei freeDays
    settings.beginGroup(REG_KEY_DATETIMEVIEW_FREEDAYSGROUP);
    Days days = {
        settings.value(REG_KEY_DATETIMEVIEW_MON).toBool(),
        settings.value(REG_KEY_DATETIMEVIEW_TUE).toBool(),
        settings.value(REG_KEY_DATETIMEVIEW_WED).toBool(),
        settings.value(REG_KEY_DATETIMEVIEW_THU).toBool(),
        settings.value(REG_KEY_DATETIMEVIEW_FRI).toBool(),
        settings.value(REG_KEY_DATETIMEVIEW_SAT).toBool(),
        settings.value(REG_KEY_DATETIMEVIEW_SUN).toBool(),
    };
    QColor color = settings.value(REG_KEY_DATETIMEVIEW_COLOR).value<QColor>();
    ganttController->SetFreeDays(days);
    ganttController->SetFreeDaysColor(color);
    settings.endGroup();
    settings.endGroup();

    // Disabilito alcune voci dal menu se non ho caricato un progetto
    if(this->projectModel == nullptr)
    {
        ui->actionAdd_Task_Group->setEnabled(false);
        ui->actionAdd_Task->setEnabled(false);
        ui->actionAdd_Milestone->setEnabled(false);
        ui->action_Edit_Project->setEnabled(false);
        ui->action_Edit_Task_Group->setEnabled(false);
        ui->action_Edit_Task->setEnabled(false);
        ui->action_Edit_Milestone->setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    delete dateTimeGrid;
    delete ganttController;
    delete freeDaysModel;
    delete projectModel;
    delete viewModel;
    delete costraintModel;
    delete ui;
}

void MainWindow::initGanttView()
{
    // Set initial sizes for GanttView
    QList<int> splitList;
    splitList << (ui->ganttView->width()/2) \
              << (ui->ganttView->width()/2);
    ui->ganttView->splitter()->setSizes(splitList);

    /***************************
    *   TODO : subclass and use headerwidget to
    *   paint in english??
    *
    ********************************/

    dateTimeGrid = new KDGantt::DateTimeGrid();
    dateTimeGrid->setUserDefinedLowerScale(new TeeamDateTimeScaleFormatter(*(dateTimeGrid->userDefinedLowerScale())));
    dateTimeGrid->setUserDefinedUpperScale(new TeeamDateTimeScaleFormatter(*(dateTimeGrid->userDefinedUpperScale())));
    ui->ganttView->graphicsView()->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->ganttView->setGrid( dateTimeGrid );
    ui->ganttView->graphicsView()->setHeaderContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);

    viewModel = new QStandardItemModel( 0, 6, this );
    viewModel->setHeaderData( 0, Qt::Horizontal, tr( "Project Tree View" ) );
    ui->ganttView->setModel( viewModel );

    QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
    connect(leftView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_actionTreeView_doubleclick(const QModelIndex&)));
    leftView->setExpandsOnDoubleClick(false);
    leftView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    leftView->setColumnHidden( 1, true );
    leftView->setColumnHidden( 2, true );
    leftView->setColumnHidden( 3, true );
    leftView->setColumnHidden( 4, true );
    leftView->setColumnHidden( 5, true );
    leftView->header()->setStretchLastSection( true );

     QFont font;
     font.setPixelSize(15);
     leftView->setFont(font);

    QSettings settings;
    settings.beginGroup(REG_KEY_DATETIMEVIEW);
    if(settings.contains(REG_KEY_DATETIMEVIEW_DAYWIDTH))
    {
        int width = settings.value(REG_KEY_DATETIMEVIEW_DAYWIDTH).toInt();
        dateTimeGrid->setDayWidth(width);
    }
    if(settings.contains(REG_KEY_DATETIMEVIEW_SCALE))
    {
        KDGantt::DateTimeGrid::Scale scale = static_cast<KDGantt::DateTimeGrid::Scale>(settings.value(REG_KEY_DATETIMEVIEW_SCALE).toInt());
        dateTimeGrid->setScale(scale);
    }
    settings.endGroup();

    ui->ganttView->leftView()->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );

    ui->ganttView->installEventFilter(this);
}

void MainWindow::UpdateView()
{
    if(freeDaysModel->isChanged())
    {
        UpdateFreeDaysView();
    }

    if(projectModel != nullptr)
    {
        if(projectModel->isChanged())
        {
            if(projectModel->IsProjectChanged())
            {
                UpdateProjectView();
            }

            if(projectModel->IsTaskGroupChanged())
            {
                UpdateTaskGroupView();
            }

            // TODO : refactor??
            if(projectModel->IsEntitiesListChanged())
            {
                UpdateEntitiesView();
            }
        }
    }

    // TODO : sostituire con bottone dedicato o con controllo più performante?
    //QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
    //leftView->expandAll();

    return;
}

void MainWindow::UpdateFreeDaysView()
{
    QSet<Qt::DayOfWeek> days;
    if(freeDaysModel->GetFreeDays().bMonday)
        days.insert(Qt::Monday);
    if(freeDaysModel->GetFreeDays().bTuesday)
        days.insert(Qt::Tuesday);
    if(freeDaysModel->GetFreeDays().bWednesday)
        days.insert(Qt::Wednesday);
    if(freeDaysModel->GetFreeDays().bThursday)
        days.insert(Qt::Thursday);
    if(freeDaysModel->GetFreeDays().bFriday)
        days.insert(Qt::Friday);
    if(freeDaysModel->GetFreeDays().bSaturday)
        days.insert(Qt::Saturday);
    if(freeDaysModel->GetFreeDays().bSunday)
        days.insert(Qt::Sunday);

    dateTimeGrid->setFreeDays(days);

    QBrush brush(freeDaysModel->GetFreeDaysColor());
    dateTimeGrid->setFreeDaysBrush(brush);
}

void MainWindow::UpdateProjectView()
{
    // Abilito la scrollbar verticale
    ui->ganttView->graphicsView()->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );

    if(projectModel->IsNew())
    {
        viewModel = new QStandardItemModel( 0, 6, this );
        viewModel->setHeaderData( 0, Qt::Horizontal, tr( "Project Tree View" ) );
        ui->ganttView->setModel( viewModel );

        if (viewModel->rowCount() == 0)
        {
            if ( !viewModel->insertRow( 0 ) )
                return;
        }
    }

    viewModel->setData( viewModel->index( 0, 0 ), projectModel->GetName() );
    viewModel->setData( viewModel->index( 0, 1 ), KDGantt::TypeSummary );
    const QString legend( "" );
    if ( ! legend.isEmpty() )
        viewModel->setData( viewModel->index( 0, 5 ), legend );

    // Espando questo nodo
    //QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
    //leftView->expand(viewModel->index( 0, 0 ));
}

void MainWindow::UpdateTaskGroupView()
{
    for(int i = 0; i < projectModel->GetTaskGroup().length(); i++)
    {
        const QModelIndex projectIndex = viewModel->index(0,0);

        if(projectModel->GetTaskGroup().at(i)->IsNew())
        {
            // Ogni nuovo
            if ( !viewModel->insertRow( i, projectIndex ) )
                return;

            int row = i;
            if ( row == 0 && projectIndex.isValid() )
                viewModel->insertColumns( viewModel->columnCount( projectIndex ), 5, projectIndex );

            viewModel->setData( viewModel->index( row, 0, projectIndex ), projectModel->GetTaskGroup().at(i)->getName() );
            viewModel->setData( viewModel->index( row, 1, projectIndex ), KDGantt::TypeSummary );
            const QString legend( "" );
            if ( ! legend.isEmpty() )
                viewModel->setData( viewModel->index( row, 5, projectIndex ), legend );

            QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
            leftView->expand(projectIndex);

        }
        else if(projectModel->GetTaskGroup().at(i)->isChanged())
        {
            // Controllo se ho fatto modifiche al group
            if(projectModel->GetTaskGroup().at(i)->IsGroupChanged())
            {
                int row = i;

                viewModel->setData( viewModel->index( row, 0, projectIndex ), projectModel->GetTaskGroup().at(i)->getName() );
                viewModel->setData( viewModel->index( row, 1, projectIndex ), KDGantt::TypeSummary );

                QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
                leftView->expand(projectIndex);
            }

            // Se ho aggiunto un task a una lista:
            for (int j = 0; j < projectModel->GetTaskGroup().at(i)->GetEntitiesList().length(); j++)
            {
                const QModelIndex parent = viewModel->index(i, 0, projectIndex);

                bool isNew = projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(j)->IsNew();
                bool isChanged = projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(j)->isChanged();
                bool isRemoved = projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(j)->IsRemoved();

                if(isNew || isChanged)
                {
                    if(isNew)
                    {
                        if ( !viewModel->insertRow( j, parent ) )
                            return;
                    }

                    int row = j;
                    if ( row == 0 && projectIndex.isValid() && isNew)
                        viewModel->insertColumns( viewModel->columnCount( parent ), 5, parent );

                    if(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(j)->getEntityType() == TASK_CODE)
                    {
                        viewModel->setData( viewModel->index( row, 0, parent ), static_cast<Task *>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(j))->getName() );
                        viewModel->setData( viewModel->index( row, 1, parent ), KDGantt::TypeTask );
                        viewModel->setData( viewModel->index( row, 2, parent ), static_cast<Task *>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(j))->getStart(), KDGantt::StartTimeRole );
                        viewModel->setData( viewModel->index( row, 3, parent ), static_cast<Task *>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(j))->getEnd(), KDGantt::EndTimeRole );
                        viewModel->setData( viewModel->index( row, 4, parent ), static_cast<Task *>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(j))->getCompletition() );
                        const QString legend( "" );
                        if ( ! legend.isEmpty() )
                            viewModel->setData( viewModel->index( row, 5, parent ), legend );
                    }
                    else if(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(j)->getEntityType() == MILESTONE_CODE)
                    {
                        viewModel->setData( viewModel->index( row, 0, parent ), static_cast<Milestone *>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(j))->getName() );
                        viewModel->setData( viewModel->index( row, 1, parent ), KDGantt::TypeEvent );
                        viewModel->setData( viewModel->index( row, 2, parent ), static_cast<Milestone *>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(j))->getDateTime(), KDGantt::StartTimeRole );
                        viewModel->setData( viewModel->index( row, 3, parent ), static_cast<Milestone *>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(j))->getDateTime(), KDGantt::EndTimeRole );
                        const QString legend( "" );
                        if ( ! legend.isEmpty() )
                            viewModel->setData( viewModel->index( row, 5, parent ), legend );
                    }

                    if(isNew)
                    {
                        QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
                        leftView->expand(projectIndex);
                        leftView->expand(parent);
                    }
                }
                else if(isRemoved)
                {
                    viewModel->removeRow(j, parent);
                    qobject_cast<QTreeView*>( ui->ganttView->leftView() )->clearSelection();
                }
            }
        }
        else if(projectModel->GetTaskGroup().at(i)->IsRemoved())
        {
            viewModel->removeRow(i, projectIndex);
            qobject_cast<QTreeView*>( ui->ganttView->leftView() )->clearSelection();
        }
    }
}

void MainWindow::UpdateEntitiesView()
{
    for(int i = 0; i < projectModel->GetEntitiesList().length(); i++)
    {
        const QModelIndex projectIndex = viewModel->index(0,0);

        bool isNew = projectModel->GetEntitiesList().at(i)->IsNew();
        bool isChanged = projectModel->GetEntitiesList().at(i)->isChanged();
        bool isRemoved = projectModel->GetEntitiesList().at(i)->IsRemoved();

        if(isNew || isChanged)
        {
            // Ogni nuovo task/milestone lo aggiungo al suo posto dopo i task group
            if(isNew)
            {
                if ( !viewModel->insertRow( projectModel->GetTaskGroup().length() + i, projectIndex ) )
                    return;
            }

            int row = projectModel->GetTaskGroup().length() + i;
            if ( row == 0 && projectIndex.isValid() && isNew )
                viewModel->insertColumns( viewModel->columnCount( projectIndex ), 5, projectIndex );

            if(projectModel->GetEntitiesList().at(i)->getEntityType() == TASK_CODE)
            {
                viewModel->setData( viewModel->index( row, 0, projectIndex ), static_cast<Task *>(projectModel->GetEntitiesList().at(i))->getName() );
                viewModel->setData( viewModel->index( row, 1, projectIndex ), KDGantt::TypeTask );
                viewModel->setData( viewModel->index( row, 2, projectIndex ), static_cast<Task *>(projectModel->GetEntitiesList().at(i))->getStart(), KDGantt::StartTimeRole );
                viewModel->setData( viewModel->index( row, 3, projectIndex ), static_cast<Task *>(projectModel->GetEntitiesList().at(i))->getEnd(), KDGantt::EndTimeRole );
                viewModel->setData( viewModel->index( row, 4, projectIndex ), static_cast<Task *>(projectModel->GetEntitiesList().at(i))->getCompletition() );
                const QString legend( "" );
                if ( ! legend.isEmpty() )
                    viewModel->setData( viewModel->index( row, 5, projectIndex ), legend );
            }
            else if(projectModel->GetEntitiesList().at(i)->getEntityType() == MILESTONE_CODE)
            {
                viewModel->setData( viewModel->index( row, 0, projectIndex ), static_cast<Milestone *>(projectModel->GetEntitiesList().at(i))->getName() );
                viewModel->setData( viewModel->index( row, 1, projectIndex ), KDGantt::TypeEvent );
                viewModel->setData( viewModel->index( row, 2, projectIndex ), static_cast<Milestone *>(projectModel->GetEntitiesList().at(i))->getDateTime(), KDGantt::StartTimeRole );
                viewModel->setData( viewModel->index( row, 3, projectIndex ), static_cast<Milestone *>(projectModel->GetEntitiesList().at(i))->getDateTime(), KDGantt::EndTimeRole );
                const QString legend( "" );
                if ( ! legend.isEmpty() )
                    viewModel->setData( viewModel->index( row, 5, projectIndex ), legend );
            }

            if(isNew)
            {
                QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
                leftView->expand(projectIndex);
            }
        }
        else if(isRemoved)
        {
            viewModel->removeRow(projectModel->GetTaskGroup().length() + i, projectIndex);
            qobject_cast<QTreeView*>( ui->ganttView->leftView() )->clearSelection();
        }
    }
}

void MainWindow::on_actionNew_Project_triggered()
{
    AddProjectDialog *dialog = new AddProjectDialog( this );
    if ( dialog->exec() == QDialog::Rejected || !dialog ) {
        delete dialog;
        return;
    }

    // TODO : re-inizializzo il progetto, devo salvare quello vecchio e agganciarmi al nuovo
    TeeamProject *newProject = new TeeamProject(dialog->GetProjectName(), dialog->GetPeopleList());
    this->projectModel = newProject;
    newProject->attach(this);
    ganttController->NewProject(newProject);

    // Abilito alcune voci del menu
    ui->actionAdd_Task_Group->setEnabled(true);
    ui->actionAdd_Task->setEnabled(true);
    ui->actionAdd_Milestone->setEnabled(true);
    ui->action_Edit_Project->setEnabled(true);
    ui->action_Edit_Task_Group->setEnabled(true);
    ui->action_Edit_Task->setEnabled(true);
    ui->action_Edit_Milestone->setEnabled(true);

    delete dialog;
    return;
}

void MainWindow::on_actionAdd_Task_Group_triggered()
{
    QList<QString> existingGroups;
    for(int i = 0; i < projectModel->GetTaskGroup().length(); i++)
        existingGroups.append(projectModel->GetTaskGroup().at(i)->getName());

    AddTaskGroupDialog *dialog = new AddTaskGroupDialog( existingGroups, this );
    if ( dialog->exec() == QDialog::Rejected || !dialog ) {
        delete dialog;
        return;
    }

    ganttController->AddTaskGroup(this, dialog->GetTaskGroupName());

    delete dialog;
    return;
}

void MainWindow::on_actionAdd_Task_triggered()
{
    QList<QString> groupList;
    for(int i = 0; i < projectModel->GetTaskGroup().length(); i++)
        groupList << projectModel->GetTaskGroup().at(i)->getName();

    QList<QString> totalPeople = projectModel->GetPeopleList();

    AddTaskDialog *dialog = new AddTaskDialog( groupList, totalPeople, this );
    if ( dialog->exec() == QDialog::Rejected || !dialog ) {
        delete dialog;
        return;
    }

    QString taskName = dialog->GetTaskName();
    int selectedParent = dialog->GetSelectedGroup();
    QDateTime start = dialog->GetStartDateTime();
    QDateTime end = dialog->GetEndDateTime();
    QList<QString> taskPeople = dialog->GetPeople();
    int completition = dialog->GetCompletition();

    if(selectedParent > 0)
        ganttController->AddTask(this, taskName, start, end, taskPeople, completition, selectedParent);
    else
        ganttController->AddTask(this, taskName, start, end, taskPeople, completition);
    delete dialog;
    return;
}

void MainWindow::on_actionAdd_Milestone_triggered()
{
    QList<QString> groupList;
    for(int i = 0; i < projectModel->GetTaskGroup().length(); i++)
        groupList << projectModel->GetTaskGroup().at(i)->getName();

    QList<QString> totalPeople = projectModel->GetPeopleList();

    AddMilestoneDialog *dialog = new AddMilestoneDialog( groupList, totalPeople, this );
    if ( dialog->exec() == QDialog::Rejected || !dialog ) {
        delete dialog;
        return;
    }

    QString milestoneName = dialog->GetTaskName();
    int selectedParent = dialog->GetSelectedGroup();
    QDateTime start = dialog->GetStartDateTime();
    QList<QString> milestonePeople = dialog->GetPeople();

    if(selectedParent > 0)
        ganttController->AddMilestone(this, milestoneName, start, milestonePeople, selectedParent);
    else
        ganttController->AddMilestone(this, milestoneName, start, milestonePeople);
    delete dialog;
    return;
}

void MainWindow::on_actionZoom_In_triggered()
{
    qreal dayWidth = dateTimeGrid->dayWidth();
    dateTimeGrid->setScale(KDGantt::DateTimeGrid::ScaleAuto);
    if (dayWidth > 400)
        dateTimeGrid->setDayWidth(dayWidth + 40);
    else if (dayWidth > 60)
        dateTimeGrid->setDayWidth(dayWidth + 40);
    else if (dayWidth > 20)
        dateTimeGrid->setDayWidth(dayWidth + 10);
    else
        dateTimeGrid->setDayWidth(dayWidth + 5);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    qreal dayWidth = dateTimeGrid->dayWidth();
    dateTimeGrid->setScale(KDGantt::DateTimeGrid::ScaleAuto);
    if(dayWidth <= 20)
    {
        if (dayWidth <= 5)
             dateTimeGrid->setDayWidth(5);
        else
            dateTimeGrid->setDayWidth(dayWidth - 5);
    }
    else if(dayWidth <= 60)
        dateTimeGrid->setDayWidth(10);
    else if (dayWidth <= 400)
        dateTimeGrid->setDayWidth(dayWidth - 40);
    else
        dateTimeGrid->setDayWidth(dayWidth - 40);
}

void MainWindow::on_actionSet_Free_Days_triggered()
{
    FreeDaysDialog *dialog = new FreeDaysDialog( freeDaysModel->GetFreeDays(), freeDaysModel->GetFreeDaysColor(), this );
    if ( dialog->exec() == QDialog::Rejected || !dialog ) {
        delete dialog;
        return;
    }

    Days days = dialog->GetSelectedDays();
    ganttController->SetFreeDays(days);
    QColor color = dialog->GetSelectedColor();
    ganttController->SetFreeDaysColor(color);
    delete dialog;
    return;
}

void MainWindow::on_actionTreeView_doubleclick(const QModelIndex& index)
{
	QString text = "DoubleClicked row: " + QString::number(index.row()) + "; column: " + QString::number(index.column());
    qDebug() << text;

    if(index.row() == 0 && index.column() == 0 && !index.parent().isValid())
    {
        // Ho cliccato il project
        on_action_Edit_Project_triggered();
    }
    else if(index.parent().isValid() && !index.parent().parent().isValid())
    {
        if(index.row() < projectModel->GetTaskGroup().length())
        {
             // Controllo se ho cliccato un gruppo
            QList<QString> groups;
            for (int i = 0; i < projectModel->GetTaskGroup().length(); i++)
                groups << projectModel->GetTaskGroup().at(i)->getName();

            EditTaskGroupDialog *dialog = new EditTaskGroupDialog( groups, this, index.row() );
            if ( dialog->exec() == QDialog::Rejected || !dialog ) {
                delete dialog;
                return;
            }

            QString newName = dialog->GetTaskGroupName();

            if(newName != projectModel->GetTaskGroup().at(index.row())->getName())
            {
                // TODO : sistemare, non arriva l'update!!
                ganttController->EditTaskGroup(index.row(), newName);
            }

            delete dialog;
            return;
        }
        else
        {
            // oppure se ho cliccato un task/milestone non dipendente da nessun gruppo
            int entityIndex = index.row() - projectModel->GetTaskGroup().length();
            QList<QString> groups;
            for (int i = 0; i < projectModel->GetTaskGroup().length(); i++)
                groups << projectModel->GetTaskGroup().at(i)->getName();

            if(projectModel->GetEntitiesList().at(entityIndex)->getEntityType() == TASK_CODE)
            {
                EditTaskDialog *dialog = new EditTaskDialog( static_cast<Task*>(projectModel->GetEntitiesList().at(entityIndex))->getName(),
                                                             -1,
                                                             groups,
                                                             static_cast<Task*>(projectModel->GetEntitiesList().at(entityIndex))->getPeople(),
                                                             projectModel->GetPeopleList(),
                                                             static_cast<Task*>(projectModel->GetEntitiesList().at(entityIndex))->getStart(),
                                                             static_cast<Task*>(projectModel->GetEntitiesList().at(entityIndex))->getEnd(),
                                                             static_cast<Task*>(projectModel->GetEntitiesList().at(entityIndex))->getCompletition(),
                                                             this);

                if ( dialog->exec() == QDialog::Rejected || !dialog ) {
                    delete dialog;
                    return;
                }

                ganttController->EditTaskOrMilestone(dialog->GetTaskName(),
                                                     dialog->GetStartDateTime(),
                                                     dialog->GetEndDateTime(),
                                                     dialog->GetPeople(),
                                                     dialog->GetCompletition(),
                                                     dialog->GetSelectedGroup(),
                                                     entityIndex);
            }
            else if(projectModel->GetEntitiesList().at(entityIndex)->getEntityType() == MILESTONE_CODE)
            {
                EditMilestoneDialog *dialog = new EditMilestoneDialog( static_cast<Milestone*>(projectModel->GetEntitiesList().at(entityIndex))->getName(),
                                                                       -1,
                                                                       groups,
                                                                       static_cast<Milestone*>(projectModel->GetEntitiesList().at(entityIndex))->getPeople(),
                                                                       projectModel->GetPeopleList(),
                                                                       static_cast<Milestone*>(projectModel->GetEntitiesList().at(entityIndex))->getDateTime(),
                                                                       this);

                if ( dialog->exec() == QDialog::Rejected || !dialog ) {
                    delete dialog;
                    return;
                }

                ganttController->EditTaskOrMilestone(dialog->GetMilestoneName(),
                                                     dialog->GetStartDateTime(),
                                                     dialog->GetPeople(),
                                                     dialog->GetSelectedGroup(),
                                                     entityIndex);
            }
        }
    }
    else if(index.parent().parent().isValid() && !index.parent().parent().parent().isValid())
    {
        // oppure se ho cliccato un task/milestone sotto un gruppo
        int entityIndex = index.row();
        int parentIndex = index.parent().row();
        QList<QString> groups;
        for (int i = 0; i < projectModel->GetTaskGroup().length(); i++)
            groups << projectModel->GetTaskGroup().at(i)->getName();

        if(projectModel->GetTaskGroup().at(parentIndex)->GetEntitiesList().at(entityIndex)->getEntityType() == TASK_CODE)
        {
            EditTaskDialog *dialog = new EditTaskDialog( static_cast<Task*>(projectModel->GetTaskGroup().at(parentIndex)->GetEntitiesList().at(entityIndex))->getName(),
                                                         parentIndex,
                                                         groups,
                                                         static_cast<Task*>(projectModel->GetTaskGroup().at(parentIndex)->GetEntitiesList().at(entityIndex))->getPeople(),
                                                         projectModel->GetPeopleList(),
                                                         static_cast<Task*>(projectModel->GetTaskGroup().at(parentIndex)->GetEntitiesList().at(entityIndex))->getStart(),
                                                         static_cast<Task*>(projectModel->GetTaskGroup().at(parentIndex)->GetEntitiesList().at(entityIndex))->getEnd(),
                                                         static_cast<Task*>(projectModel->GetTaskGroup().at(parentIndex)->GetEntitiesList().at(entityIndex))->getCompletition(),
                                                         this);

            if ( dialog->exec() == QDialog::Rejected || !dialog ) {
                delete dialog;
                return;
            }

            ganttController->EditTaskOrMilestone(dialog->GetTaskName(),
                                                 dialog->GetStartDateTime(),
                                                 dialog->GetEndDateTime(),
                                                 dialog->GetPeople(),
                                                 dialog->GetCompletition(),
                                                 dialog->GetSelectedGroup(),
                                                 entityIndex,
                                                 parentIndex);
        }
        else if(projectModel->GetTaskGroup().at(parentIndex)->GetEntitiesList().at(entityIndex)->getEntityType() == MILESTONE_CODE)
        {
            EditMilestoneDialog *dialog = new EditMilestoneDialog( static_cast<Milestone*>(projectModel->GetTaskGroup().at(parentIndex)->GetEntitiesList().at(entityIndex))->getName(),
                                                                   parentIndex,
                                                                   groups,
                                                                   static_cast<Milestone*>(projectModel->GetTaskGroup().at(parentIndex)->GetEntitiesList().at(entityIndex))->getPeople(),
                                                                   projectModel->GetPeopleList(),
                                                                   static_cast<Milestone*>(projectModel->GetTaskGroup().at(parentIndex)->GetEntitiesList().at(entityIndex))->getDateTime(),
                                                                   this);

            if ( dialog->exec() == QDialog::Rejected || !dialog ) {
                delete dialog;
                return;
            }

            ganttController->EditTaskOrMilestone(dialog->GetMilestoneName(),
                                                 dialog->GetStartDateTime(),
                                                 dialog->GetPeople(),
                                                 dialog->GetSelectedGroup(),
                                                 entityIndex,
                                                 parentIndex);
        }
    }
}

void MainWindow::on_actionTreeView_del(const QModelIndex &index)
{
    if(index.isValid())
    {
        QString text = "Deleted row: " + QString::number(index.row()) + "; column: " + QString::number(index.column());
        qDebug() << text;

        if(index.row() == 0 && index.column() == 0 && !index.parent().isValid())
        {
            // TODO : gestione eliminazione progetto?
        }
        else if(index.parent().isValid() && !index.parent().parent().isValid())
        {
            // Controllo se ho eliminato un gruppo
            if(index.row() < projectModel->GetTaskGroup().length())
            {
                ganttController->RemoveTaskGroup(index.row());
            }
            // oppure se ho eliminato un task/milestone non dipendente da nessun altro
            else
            {
                ganttController->RemoveTaskOrMilestone(index.row() - projectModel->GetTaskGroup().length());
            }
        }
        else if(index.parent().parent().isValid() && !index.parent().parent().parent().isValid())
        {
            // oppure se ho eliminato un task/milestone sotto un gruppo
            ganttController->RemoveTaskOrMilestone(index.row(), index.parent().row());
        }
    }
}

bool MainWindow::eventFilter(QObject* target, QEvent* event)
{
    if(target == ui->ganttView )
    {
        if(event->type() == QEvent::KeyPress )
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent->key() == Qt::Key_Delete )
            {
                QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView());
                on_actionTreeView_del(leftView->currentIndex());
                leftView->setCurrentIndex(viewModel->index(0,0)); // Per disabilitare la possibilità di cancellare mezzo progetto
                leftView->clearSelection();                       // premendo "Canc" più volte
            }
        }
    }

    return QMainWindow::eventFilter(target, event);
}

void MainWindow::closeEvent(QCloseEvent *eventArgs)
{
    QSettings settings;

    settings.beginGroup(REG_KEY_MAINWINDOW);
    settings.setValue(REG_KEY_MAINWINDOW_MAXIMIZED, isMaximized());
    settings.setValue(REG_KEY_MAINWINDOW_SIZE, size());
    settings.endGroup();

    settings.beginGroup(REG_KEY_DATETIMEVIEW);
    settings.beginGroup(REG_KEY_DATETIMEVIEW_FREEDAYSGROUP);
    settings.setValue(REG_KEY_DATETIMEVIEW_MON, dateTimeGrid->freeDays().contains(Qt::Monday));
    settings.setValue(REG_KEY_DATETIMEVIEW_TUE, dateTimeGrid->freeDays().contains(Qt::Tuesday));
    settings.setValue(REG_KEY_DATETIMEVIEW_WED, dateTimeGrid->freeDays().contains(Qt::Wednesday));
    settings.setValue(REG_KEY_DATETIMEVIEW_THU, dateTimeGrid->freeDays().contains(Qt::Thursday));
    settings.setValue(REG_KEY_DATETIMEVIEW_FRI, dateTimeGrid->freeDays().contains(Qt::Friday));
    settings.setValue(REG_KEY_DATETIMEVIEW_SAT, dateTimeGrid->freeDays().contains(Qt::Saturday));
    settings.setValue(REG_KEY_DATETIMEVIEW_SUN, dateTimeGrid->freeDays().contains(Qt::Sunday));
    settings.setValue(REG_KEY_DATETIMEVIEW_COLOR, dateTimeGrid->freeDaysBrush().color());
    settings.endGroup();
    settings.setValue(REG_KEY_DATETIMEVIEW_DAYWIDTH, dateTimeGrid->dayWidth());
    settings.setValue(REG_KEY_DATETIMEVIEW_SCALE, dateTimeGrid->scale());
    settings.endGroup();

    eventArgs->accept();
}

void MainWindow::on_action_Quit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_action_Edit_Project_triggered()
{
    if(projectModel != nullptr)
    {
        AddProjectDialog *dialog = new AddProjectDialog( projectModel->GetName(), projectModel->GetPeopleList(), this );
        dialog->setWindowTitle("Edit Project");
        if ( dialog->exec() == QDialog::Rejected || !dialog ) {
            delete dialog;
            return;
        }

        QString newName = dialog->GetProjectName();
        QList<QString> newPeople = dialog->GetPeopleList();

        if(newName != projectModel->GetName() || newPeople != projectModel->GetPeopleList())
        {
            ganttController->EditProject(newName, newPeople);
        }

        delete dialog;
        return;
    }
}

void MainWindow::on_action_Edit_Task_Group_triggered()
{
    if(projectModel != nullptr)
    {
        QList<QString> groups;
        for (int i = 0; i < projectModel->GetTaskGroup().length(); i++)
            groups << projectModel->GetTaskGroup().at(i)->getName();

        EditTaskGroupDialog *dialog = new EditTaskGroupDialog( groups, this );
        if ( dialog->exec() == QDialog::Rejected || !dialog ) {
            delete dialog;
            return;
        }

        QString newName = dialog->GetTaskGroupName();
        int selectedGroup = dialog->GetSelectedGroup();

        if(newName != projectModel->GetTaskGroup().at(selectedGroup-1)->getName())
        {
            ganttController->EditTaskGroup(selectedGroup, newName);
        }

        delete dialog;
        return;
    }
}

void MainWindow::on_action_Edit_Task_triggered()
{
    // TODO : implementare!!
}

void MainWindow::on_action_Edit_Milestone_triggered()
{
    // TODO : implementare!!
}

void MainWindow::on_action_Save_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                       tr("Save Teeam Project"), ".",
                                       tr("Teeam files (*.tmproj)"));


    QFile file(filename);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    // Inizio il salvataggio del progetto
    xmlWriter.writeStartElement(KEY_PROJECT);

    // Salvo il nome
    xmlWriter.writeTextElement(KEY_NAME, projectModel->GetName() );

    // Salvo l'elenco di persone
    for(int i = 0; i < projectModel->GetPeopleList().length(); i++)
        xmlWriter.writeTextElement(KEY_PERSON, projectModel->GetPeopleList().at(i));

    // Salvo i gruppi
    for(int i = 0; i < projectModel->GetTaskGroup().length(); i++)
    {
        xmlWriter.writeStartElement(KEY_GROUP);
        xmlWriter.writeTextElement(KEY_NAME, projectModel->GetTaskGroup().at(i)->getName() );
        for(int j = 0; j < projectModel->GetTaskGroup().at(i)->GetEntitiesList().length(); j++)
        {
            if(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(i)->getEntityType() == TASK_CODE)
            {
                xmlWriter.writeTextElement(KEY_ENTITYTYPE, KEY_TASKTYPE);
                xmlWriter.writeTextElement(KEY_NAME, static_cast<Task*>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(i))->getName());

                for(int k = 0; k < static_cast<Task*>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(i))->getPeople().length(); k++)
                    xmlWriter.writeTextElement(KEY_PERSON, static_cast<Task*>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(i))->getPeople().at(k));

                xmlWriter.writeTextElement(KEY_STARTDATETIME, static_cast<Task*>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(i))->getStart().toString());
                xmlWriter.writeTextElement(KEY_ENDDATETIME, static_cast<Task*>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(i))->getEnd().toString());

                xmlWriter.writeTextElement(KEY_COMPLETITION, QString::number(static_cast<Task*>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(i))->getCompletition()));
            }
            else if(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(i)->getEntityType() == MILESTONE_CODE)
            {
                xmlWriter.writeTextElement(KEY_ENTITYTYPE, KEY_MILESTONETYPE);
                xmlWriter.writeTextElement(KEY_NAME, static_cast<Milestone*>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(i))->getName());

                for(int k = 0; k < static_cast<Milestone*>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(i))->getPeople().length(); k++)
                    xmlWriter.writeTextElement(KEY_PERSON, static_cast<Milestone*>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(i))->getPeople().at(k));

                xmlWriter.writeTextElement(KEY_STARTDATETIME, static_cast<Milestone*>(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(i))->getDateTime().toString());
            }
        }
        xmlWriter.writeEndElement();
    }

    // Salvo i task/milestone fuori dai gruppi
    for(int i = 0; i < projectModel->GetTaskGroup().length(); i++)
    {
        xmlWriter.writeStartElement(KEY_ENTITY);
        if(projectModel->GetEntitiesList().at(i)->getEntityType() == TASK_CODE)
        {
            xmlWriter.writeTextElement(KEY_ENTITYTYPE, KEY_TASKTYPE);
            xmlWriter.writeTextElement(KEY_NAME, static_cast<Task*>(projectModel->GetEntitiesList().at(i))->getName());

            for(int k = 0; k < static_cast<Task*>(projectModel->GetEntitiesList().at(i))->getPeople().length(); k++)
                xmlWriter.writeTextElement(KEY_PERSON, static_cast<Task*>(projectModel->GetEntitiesList().at(i))->getPeople().at(k));

            xmlWriter.writeTextElement(KEY_STARTDATETIME, static_cast<Task*>(projectModel->GetEntitiesList().at(i))->getStart().toString());
            xmlWriter.writeTextElement(KEY_ENDDATETIME, static_cast<Task*>(projectModel->GetEntitiesList().at(i))->getEnd().toString());

            xmlWriter.writeTextElement(KEY_COMPLETITION, QString::number(static_cast<Task*>(projectModel->GetEntitiesList().at(i))->getCompletition()));
        }
        else if(projectModel->GetEntitiesList().at(i)->getEntityType() == MILESTONE_CODE)
        {
            xmlWriter.writeTextElement(KEY_ENTITYTYPE, KEY_MILESTONETYPE);
            xmlWriter.writeTextElement(KEY_NAME, static_cast<Milestone*>(projectModel->GetEntitiesList().at(i))->getName());

            for(int k = 0; k < static_cast<Milestone*>(projectModel->GetEntitiesList().at(i))->getPeople().length(); k++)
                xmlWriter.writeTextElement(KEY_PERSON, static_cast<Milestone*>(projectModel->GetEntitiesList().at(i))->getPeople().at(k));

            xmlWriter.writeTextElement(KEY_STARTDATETIME, static_cast<Milestone*>(projectModel->GetEntitiesList().at(i))->getDateTime().toString());
        }
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();

    file.close();
}

void MainWindow::on_actionOpen_File_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                       tr("Open Teeam Project"), ".",
                                       tr("Teeam files (*.tmproj)"));

    setCursor(Qt::WaitCursor);

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        setCursor(Qt::ArrowCursor);
        return;
    }

    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    if(xmlReader.readNext() == QXmlStreamReader::Invalid)
    {
        setCursor(Qt::ArrowCursor);
        return;
    }
    // Controllo che l'elemento più alto sia un project
    if(xmlReader.isStartElement())
    {
        if(xmlReader.name() == KEY_PROJECT)
        {
            QString name;
            QStringList people;
            xmlReader.readNext();
            if(xmlReader.name() == KEY_NAME)
                name = xmlReader.readElementText();

            xmlReader.readNext();
            while(xmlReader.name() == KEY_PERSON)
            {
                people << xmlReader.readElementText();
                xmlReader.readNext();
            }

            TeeamProject *tempProj = new TeeamProject(name, people);
            ganttController->NewProject(tempProj);

            while(!xmlReader.atEnd())
            {
                if(xmlReader.name() == KEY_GROUP)
                {
                   xmlReader.readNext();
                   if(xmlReader.name() == KEY_NAME)
                   {
                        ganttController->AddTaskGroup(this, xmlReader.readElementText());
                        xmlReader.readNext();
                        if(xmlReader.name() == KEY_ENTITY)
                        {
                            xmlReader.readNext();
                            // Leggo le entity appartenenti a ogni gruppo, devo differenziare tra task e milestone
                        }
                   }
                }

                if(xmlReader.name() == KEY_ENTITY)
                {

                }
            }
        }
    }

    setCursor(Qt::ArrowCursor);

    /*

    Rxml.setDevice(&file);
    Rxml.readNext();

    while(!Rxml.atEnd())
    {
        if(Rxml.isStartElement())
        {
            if(Rxml.name() == "LAMPS")
            {
                Rxml.readNext();
            }
            else if(Rxml.name() == "LIGHT1")
            {
                while(!Rxml.atEnd())
                            {
                             if(Rxml.isEndElement())
                             {
                             Rxml.readNext();
                             break;
                             }
                             else if(Rxml.isCharacters())
                             {
                             Rxml.readNext();
                             }
                             else if(Rxml.isStartElement())
                             {
                             if(Rxml.name() == "State")
                             {
                              ReadStateElement();
                             }
                             else if(Rxml.name() == "Room")
                             {
                              ReadRoomElement();
                             }
                             else if(Rxml.name() == "Potencial")
                             {
                                  ReadPotencialElement();
                             }
                             Rxml.readNext();
                         }
                         else
                         {
                         Rxml.readNext();
                         }
                    }
            }
        }
    else
    {
        Rxml.readNext();
    }

    file.close();

        if (Rxml.hasError())
    {
       std::cerr << "Error: Failed to parse file "
                 << qPrintable(filename) << ": "
                 << qPrintable(Rxml.errorString()) << std::endl;
        }
    else if (file.error() != QFile::NoError)
    {
        std::cerr << "Error: Cannot read file " << qPrintable(filename)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
    } */
}
