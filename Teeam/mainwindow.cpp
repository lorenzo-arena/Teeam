#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "addprojectdialog.h"
#include "addtaskgroupdialog.h"
#include "addtaskdialog.h"
#include "addmilestonedialog.h"
#include "freedaysdialog.h"

#include <QApplication>
#include <QList>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTreeView>
#include <QSettings>
#include <QCloseEvent>
#include <QSet>
#include <QLocale>

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
    settings.beginGroup(KEY_MAINWINDOW);
    if(settings.contains(KEY_MAINWINDOW_MAXIMIZED))    // setto la dimensione della finestra
    {
        if(settings.value(KEY_MAINWINDOW_MAXIMIZED).toBool())
            showMaximized();
    }
    else if (settings.contains(KEY_MAINWINDOW_SIZE))
    {
        resize(settings.value(KEY_MAINWINDOW_SIZE).toSize());
    }
    settings.endGroup();

    settings.beginGroup(KEY_DATETIMEVIEW);
    // lancio un update dei freeDays
    settings.beginGroup(KEY_DATETIMEVIEW_FREEDAYSGROUP);
    Days days = {
        settings.value(KEY_DATETIMEVIEW_MON).toBool(),
        settings.value(KEY_DATETIMEVIEW_TUE).toBool(),
        settings.value(KEY_DATETIMEVIEW_WED).toBool(),
        settings.value(KEY_DATETIMEVIEW_THU).toBool(),
        settings.value(KEY_DATETIMEVIEW_FRI).toBool(),
        settings.value(KEY_DATETIMEVIEW_SAT).toBool(),
        settings.value(KEY_DATETIMEVIEW_SUN).toBool(),
    };
    QColor color = settings.value(KEY_DATETIMEVIEW_COLOR).value<QColor>();
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
    ui->ganttView->setGrid( dateTimeGrid );
    ui->ganttView->graphicsView()->setHeaderContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);

    viewModel = new QStandardItemModel( 0, 6, this );
    viewModel->setHeaderData( 0, Qt::Horizontal, tr( "Project Tree View" ) );
    ui->ganttView->setModel( viewModel );


    QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
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
    settings.beginGroup(KEY_DATETIMEVIEW);
    if(settings.contains(KEY_DATETIMEVIEW_DAYWIDTH))
    {
        int width = settings.value(KEY_DATETIMEVIEW_DAYWIDTH).toInt();
        dateTimeGrid->setDayWidth(width);
    }
    if(settings.contains(KEY_DATETIMEVIEW_SCALE))
    {
        KDGantt::DateTimeGrid::Scale scale = static_cast<KDGantt::DateTimeGrid::Scale>(settings.value(KEY_DATETIMEVIEW_SCALE).toInt());
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
    QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
    leftView->expandAll();

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
    viewModel = new QStandardItemModel( 0, 6, this );
    viewModel->setHeaderData( 0, Qt::Horizontal, tr( "Project Tree View" ) );
    ui->ganttView->setModel( viewModel );

    if (viewModel->rowCount() == 0)
    {
        if ( !viewModel->insertRow( 0 ) )
            return;
    }

    viewModel->setData( viewModel->index( 0, 0 ), projectModel->GetName() );
    viewModel->setData( viewModel->index( 0, 1 ), KDGantt::TypeSummary );
    const QString legend( "" );
    if ( ! legend.isEmpty() )
        viewModel->setData( viewModel->index( 0, 5 ), legend );
}

void MainWindow::UpdateTaskGroupView()
{
    // Non ci arriva nemmeno in debug!!
    for(int i = 0; i < projectModel->GetTaskGroup().length(); i++)
    {
        const QModelIndex projectIndex = viewModel->index(0,0);

        if(projectModel->GetTaskGroup().at(i)->IsNew())
        {
            if ( !viewModel->insertRow( i, projectIndex ) )
                return;

            int row = i;
            if ( row == 0 && projectIndex.isValid() )
                viewModel->insertColumns( viewModel->columnCount( projectIndex ), 5, projectIndex );

            viewModel->setData( viewModel->index( row, 0, projectIndex ), projectModel->GetTaskGroup().at(i)->getName() );
            viewModel->setData( viewModel->index( row, 1, projectIndex ), KDGantt::TypeSummary );
            //viewModel->setData( viewModel->index( row, 2, projectIndex ), startdt, KDGantt::StartTimeRole );
            //viewModel->setData( viewModel->index( row, 3, projectIndex ), enddt, KDGantt::EndTimeRole );
            viewModel->setData( viewModel->index( row, 4, projectIndex ), 10 );
            const QString legend( "" );
            if ( ! legend.isEmpty() )
                viewModel->setData( viewModel->index( row, 5, projectIndex ), legend );
        }
        //else if(projectModel->GetTaskGroup().at(i)->isChanged())
        else
        {
            // Se ho aggiunto un task a una lista:
            for (int j = 0; j < projectModel->GetTaskGroup().at(i)->GetEntitiesList().length(); j++)
            {
                if(projectModel->GetTaskGroup().at(i)->GetEntitiesList().at(j)->IsNew())
                {
                    const QModelIndex parent = viewModel->index(i,0,projectIndex);

                    if ( !viewModel->insertRow( j, parent ) )
                        return;

                    int row = j;
                    if ( row == 0 && projectIndex.isValid() )
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
                    else
                        return;
                }
            }
        }
    }
}

void MainWindow::UpdateEntitiesView()
{
    for(int i = 0; i < projectModel->GetEntitiesList().length(); i++)
    {
        if(projectModel->GetEntitiesList().at(i)->IsNew())
        {
            const QModelIndex parent = viewModel->index(0,0);

            if ( !viewModel->insertRow( projectModel->GetTaskGroup().length(), parent ) )
                return;

            int row = projectModel->GetTaskGroup().length() + i;
            if ( row == 0 && parent.isValid() )
                viewModel->insertColumns( viewModel->columnCount( parent ), 5, parent );


            if(projectModel->GetEntitiesList().at(i)->getEntityType() == TASK_CODE)
            {
                viewModel->setData( viewModel->index( row, 0, parent ), static_cast<Task *>(projectModel->GetEntitiesList().at(i))->getName() );
                viewModel->setData( viewModel->index( row, 1, parent ), KDGantt::TypeTask );
                viewModel->setData( viewModel->index( row, 2, parent ), static_cast<Task *>(projectModel->GetEntitiesList().at(i))->getStart(), KDGantt::StartTimeRole );
                viewModel->setData( viewModel->index( row, 3, parent ), static_cast<Task *>(projectModel->GetEntitiesList().at(i))->getEnd(), KDGantt::EndTimeRole );
                viewModel->setData( viewModel->index( row, 4, parent ), static_cast<Task *>(projectModel->GetEntitiesList().at(i))->getCompletition() );
                const QString legend( "" );
                if ( ! legend.isEmpty() )
                    viewModel->setData( viewModel->index( row, 5, parent ), legend );
            }
            else if(projectModel->GetEntitiesList().at(i)->getEntityType() == MILESTONE_CODE)
            {
                viewModel->setData( viewModel->index( row, 0, parent ), static_cast<Milestone *>(projectModel->GetEntitiesList().at(i))->getName() );
                viewModel->setData( viewModel->index( row, 1, parent ), KDGantt::TypeEvent );
                viewModel->setData( viewModel->index( row, 2, parent ), static_cast<Milestone *>(projectModel->GetEntitiesList().at(i))->getDateTime(), KDGantt::StartTimeRole );
                viewModel->setData( viewModel->index( row, 3, parent ), static_cast<Milestone *>(projectModel->GetEntitiesList().at(i))->getDateTime(), KDGantt::EndTimeRole );
                const QString legend( "" );
                if ( ! legend.isEmpty() )
                    viewModel->setData( viewModel->index( row, 5, parent ), legend );
            }
            else
                return;
        }
        else if(projectModel->GetEntitiesList().at(i)->isChanged())
        {
            // TODO : da utilizzare!
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
    if (dayWidth > 400)
    {
        dateTimeGrid->setScale(KDGantt::DateTimeGrid::ScaleHour);
        dateTimeGrid->setDayWidth(dayWidth + 40);
    }
    else if (dayWidth > 60)
    {
        dateTimeGrid->setScale(KDGantt::DateTimeGrid::ScaleDay);
        dateTimeGrid->setDayWidth(dayWidth + 40);
    }
    else if (dayWidth > 20)
    {
        dateTimeGrid->setScale(KDGantt::DateTimeGrid::ScaleWeek);
        dateTimeGrid->setDayWidth(dayWidth + 10);
    }
    else
        dateTimeGrid->setDayWidth(dayWidth + 5);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    qreal dayWidth = dateTimeGrid->dayWidth();
    if(dayWidth <= 20)
    {
        dateTimeGrid->setScale(KDGantt::DateTimeGrid::ScaleMonth);
        if (dayWidth <= 5)
             dateTimeGrid->setDayWidth(5);
        else
            dateTimeGrid->setDayWidth(dayWidth - 5);
    }
    else if(dayWidth <= 60)
    {
        dateTimeGrid->setScale(KDGantt::DateTimeGrid::ScaleWeek);
        dateTimeGrid->setDayWidth(10);
    }
    else if (dayWidth <= 400)
    {
        dateTimeGrid->setScale(KDGantt::DateTimeGrid::ScaleDay);
        dateTimeGrid->setDayWidth(dayWidth - 40);
    }
    else
    {
        dateTimeGrid->setDayWidth(dayWidth - 40);
    }
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

void MainWindow::closeEvent(QCloseEvent *eventArgs)
{
    QSettings settings;

    settings.beginGroup(KEY_MAINWINDOW);
    settings.setValue(KEY_MAINWINDOW_MAXIMIZED, isMaximized());
    settings.setValue(KEY_MAINWINDOW_SIZE, size());
    settings.endGroup();

    settings.beginGroup(KEY_DATETIMEVIEW);  
    settings.beginGroup(KEY_DATETIMEVIEW_FREEDAYSGROUP);
    settings.setValue(KEY_DATETIMEVIEW_MON, dateTimeGrid->freeDays().contains(Qt::Monday));
    settings.setValue(KEY_DATETIMEVIEW_TUE, dateTimeGrid->freeDays().contains(Qt::Tuesday));
    settings.setValue(KEY_DATETIMEVIEW_WED, dateTimeGrid->freeDays().contains(Qt::Wednesday));
    settings.setValue(KEY_DATETIMEVIEW_THU, dateTimeGrid->freeDays().contains(Qt::Thursday));
    settings.setValue(KEY_DATETIMEVIEW_FRI, dateTimeGrid->freeDays().contains(Qt::Friday));
    settings.setValue(KEY_DATETIMEVIEW_SAT, dateTimeGrid->freeDays().contains(Qt::Saturday));
    settings.setValue(KEY_DATETIMEVIEW_SUN, dateTimeGrid->freeDays().contains(Qt::Sunday));
    settings.setValue(KEY_DATETIMEVIEW_COLOR, dateTimeGrid->freeDaysBrush().color());
    settings.endGroup();
    settings.setValue(KEY_DATETIMEVIEW_DAYWIDTH, dateTimeGrid->dayWidth());
    settings.setValue(KEY_DATETIMEVIEW_SCALE, dateTimeGrid->scale());
    settings.endGroup();

    eventArgs->accept();
}

void MainWindow::on_action_Quit_triggered()
{
    QApplication::quit();
}
