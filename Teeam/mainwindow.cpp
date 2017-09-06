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
#include "errors.h"

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
#include <QFileDialog>

#ifdef DATETIMESCALE_MOD
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
#endif

MainWindow::MainWindow(GanttController *ganttController, FreeDaysModel *freeDaysModel, QString appVersion, TeeamProject *projectModel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Aggancio i vari modelli
    this->freeDaysModel = freeDaysModel;
    this->freeDaysModel->attach(this);

    if(projectModel != nullptr)
    {
        this->projectModel = projectModel;
        this->projectModel->attach(this);
        bEmptyProject = false;
    }
    else
    {
        QStringList emptylist;
        TeeamProject *newProject = new TeeamProject("", emptylist);
        this->projectModel = newProject;
        newProject->attach(this);
        ganttController->NewProject(newProject);
        bEmptyProject = true;
    }

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
    if(!bEmptyProject)
    {
        DisableMenu();
    }

    ui->statusBar->showMessage(appVersion);
}

MainWindow::~MainWindow()
{
    if(dateTimeGrid != nullptr)
    {
        delete dateTimeGrid;
    }

    if(ganttController != nullptr)
    {
        delete ganttController;
    }

    if(freeDaysModel != nullptr)
    {
        delete freeDaysModel;
    }

    if(viewModel != nullptr)
    {
        delete viewModel;
    }

    if(costraintModel != nullptr)
    {
        delete costraintModel;
    }

    if(projectModel != nullptr)
    {
        delete projectModel;
    }

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

#ifdef DATETIMESCALE_MOD
    dateTimeGrid->setUserDefinedLowerScale(new TeeamDateTimeScaleFormatter(*(dateTimeGrid->userDefinedLowerScale())));
    dateTimeGrid->setUserDefinedUpperScale(new TeeamDateTimeScaleFormatter(*(dateTimeGrid->userDefinedUpperScale())));
#endif

    ui->ganttView->graphicsView()->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->ganttView->setGrid( dateTimeGrid );
    ui->ganttView->graphicsView()->setHeaderContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);

    viewModel = new QStandardItemModel( 0, 6, this );
    QString nameHeader = "Name";
    QString startHeader = "Start";
    QString endHeader = "End";
    QString completitionHeader = "Completition";
    viewModel->setHeaderData( 0, Qt::Horizontal, nameHeader );
    viewModel->setHeaderData( 2, Qt::Horizontal, startHeader );
    viewModel->setHeaderData( 3, Qt::Horizontal, endHeader );
    viewModel->setHeaderData( 4, Qt::Horizontal, completitionHeader );
    ui->ganttView->setModel( viewModel );

    QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
    connect(leftView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_actionTreeView_doubleclick(const QModelIndex&)));

    // Gestione context menu
    leftView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(leftView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_actionTreeView_rightclick(const QPoint&)));

    leftView->setExpandsOnDoubleClick(false);
    leftView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    leftView->setColumnHidden( 1, true );
    leftView->setColumnHidden( 2, false );
    leftView->setColumnHidden( 3, false );
    leftView->setColumnHidden( 4, false );
    leftView->setColumnHidden( 5, true );
    leftView->header()->setStretchLastSection( true );
    leftView->setColumnWidth(0, 150);
    leftView->setColumnWidth(2, 250);
    leftView->setColumnWidth(3, 250);
    leftView->setColumnWidth(4, 200);

#ifdef DRAGNDROP
    leftView->setDragEnabled(true);
    leftView->viewport()->setAcceptDrops(true);
    leftView->setDropIndicatorShown(true);
    leftView->setDragDropMode(QAbstractItemView::InternalMove);
#endif

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

    if(!bEmptyProject)
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
        QString nameHeader = "Name";
        QString startHeader = "Start";
        QString endHeader = "End";
        QString completitionHeader = "Completition";
        viewModel->setHeaderData( 0, Qt::Horizontal, nameHeader );
        viewModel->setHeaderData( 2, Qt::Horizontal, startHeader );
        viewModel->setHeaderData( 3, Qt::Horizontal, endHeader );
        viewModel->setHeaderData( 4, Qt::Horizontal, completitionHeader );
        ui->ganttView->setModel( viewModel );

        QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
        leftView->setColumnWidth(0, 150);
        leftView->setColumnWidth(2, 250);
        leftView->setColumnWidth(3, 250);
        leftView->setColumnWidth(4, 200);

        if (viewModel->rowCount() == 0)
        {
            if ( !viewModel->insertRow( 0 ) )
                return;
        }
    }

    viewModel->setData( viewModel->index( 0, 0 ), projectModel->GetName() );
    viewModel->setData( viewModel->index( 0, 1 ), KDGantt::TypeSummary );

    viewModel->itemFromIndex(viewModel->index(0, 0))->setEditable(false);

#ifdef DRAGNDROP
    viewModel->itemFromIndex(viewModel->index(0, 0))->setDragEnabled(false);
    viewModel->itemFromIndex(viewModel->index(0, 0))->setDropEnabled(false);
#endif

    // Espando questo nodo
    //QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
    //leftView->expand(viewModel->index( 0, 0 ));
}

void MainWindow::UpdateTaskGroupView()
{
    for(int i = 0; i < projectModel->GetTaskGroupListSize(); i++)
    {
        const QModelIndex projectIndex = viewModel->index(0,0);

        if(projectModel->GetTaskGroupAt(i)->IsNew())
        {
            // Ogni nuovo
            if ( !viewModel->insertRow( i, projectIndex ) )
                return;

            int row = i;
            if ( row == 0 && projectIndex.isValid() )
                viewModel->insertColumns( viewModel->columnCount( projectIndex ), 5, projectIndex );

            viewModel->setData( viewModel->index( row, 0, projectIndex ), projectModel->GetTaskGroupAt(i)->getName() );
            viewModel->setData( viewModel->index( row, 1, projectIndex ), KDGantt::TypeSummary );

            QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
            leftView->expand(projectIndex);

            viewModel->itemFromIndex(viewModel->index(row, 0, projectIndex))->setEditable(false);

#ifdef DRAGNDROP
            viewModel->itemFromIndex(viewModel->index(row, 0, projectIndex))->setDropEnabled(true);
            viewModel->itemFromIndex(viewModel->index(row, 0, projectIndex))->setDragEnabled(false);
#endif

        }
        else if(projectModel->GetTaskGroupAt(i)->isChanged())
        {
            // Controllo se ho fatto modifiche al group
            if(projectModel->GetTaskGroupAt(i)->IsGroupChanged())
            {
                int row = i;

                viewModel->setData( viewModel->index( row, 0, projectIndex ), projectModel->GetTaskGroupAt(i)->getName() );
                viewModel->setData( viewModel->index( row, 1, projectIndex ), KDGantt::TypeSummary );

                QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
                leftView->expand(projectIndex);

                viewModel->itemFromIndex(viewModel->index(row, 0, projectIndex))->setEditable(false);

#ifdef DRAGNDROP
                viewModel->itemFromIndex(viewModel->index(row, 0, projectIndex))->setDropEnabled(true);
                viewModel->itemFromIndex(viewModel->index(row, 0, projectIndex))->setDragEnabled(false);
#endif
            }

            // Se ho aggiunto un task a una lista:
            for (int j = 0; j < projectModel->GetTaskGroupAt(i)->GetEntitiesListSize(); j++)
            {
                const QModelIndex parent = viewModel->index(i, 0, projectIndex);

                bool isNew = projectModel->GetTaskGroupAt(i)->GetEntityAt(j)->IsNew();
                bool isChanged = projectModel->GetTaskGroupAt(i)->GetEntityAt(j)->isChanged();
                bool isRemoved = projectModel->GetTaskGroupAt(i)->GetEntityAt(j)->IsRemoved();

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

                    if(projectModel->GetTaskGroupAt(i)->GetEntityAt(j)->getEntityType() == Task_type)
                    {
                        viewModel->setData( viewModel->index( row, 0, parent ), static_cast<Task *>(projectModel->GetTaskGroupAt(i)->GetEntityAt(j))->getName() );
                        viewModel->setData( viewModel->index( row, 1, parent ), KDGantt::TypeTask );
						// Devo settare il modello sia per la ganttView che per la TreeView
                        viewModel->setData( viewModel->index( row, 2, parent ), static_cast<Task *>(projectModel->GetTaskGroupAt(i)->GetEntityAt(j))->getStart(), KDGantt::StartTimeRole );
                        viewModel->setData( viewModel->index( row, 2, parent ), static_cast<Task *>(projectModel->GetTaskGroupAt(i)->GetEntityAt(j))->getStart().toString("dd/MM/yyyy hh:mm") );
                        viewModel->setData( viewModel->index( row, 3, parent ), static_cast<Task *>(projectModel->GetTaskGroupAt(i)->GetEntityAt(j))->getEnd(), KDGantt::EndTimeRole );
                        viewModel->setData( viewModel->index( row, 3, parent ), static_cast<Task *>(projectModel->GetTaskGroupAt(i)->GetEntityAt(j))->getEnd().toString("dd/MM/yyyy hh:mm") );
                        viewModel->setData( viewModel->index( row, 4, parent ), static_cast<Task *>(projectModel->GetTaskGroupAt(i)->GetEntityAt(j))->getCompletition() );
                    }
                    else if(projectModel->GetTaskGroupAt(i)->GetEntityAt(j)->getEntityType() == Milestone_type)
                    {
                        viewModel->setData( viewModel->index( row, 0, parent ), static_cast<Milestone *>(projectModel->GetTaskGroupAt(i)->GetEntityAt(j))->getName() );
                        viewModel->setData( viewModel->index( row, 1, parent ), KDGantt::TypeEvent );
						// Devo settare il modello sia per la ganttView che per la TreeView
                        viewModel->setData( viewModel->index( row, 2, parent ), static_cast<Milestone *>(projectModel->GetTaskGroupAt(i)->GetEntityAt(j))->getDateTime(), KDGantt::StartTimeRole );
                        viewModel->setData( viewModel->index( row, 2, parent ), static_cast<Milestone *>(projectModel->GetTaskGroupAt(i)->GetEntityAt(j))->getDateTime().toString("dd/MM/yyyy hh:mm") );
                    }

                    viewModel->itemFromIndex(viewModel->index(row, 0, parent))->setEditable(false);

#ifdef DRAGNDROP
                    viewModel->itemFromIndex(viewModel->index(row, 0, parent))->setDragEnabled(true);
                    viewModel->itemFromIndex(viewModel->index(row, 0, parent))->setDropEnabled(false);
#endif

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
        else if(projectModel->GetTaskGroupAt(i)->IsRemoved())
        {
            viewModel->removeRow(i, projectIndex);
            qobject_cast<QTreeView*>( ui->ganttView->leftView() )->clearSelection();
        }
    }
}

void MainWindow::UpdateEntitiesView()
{
    for(int i = 0; i < projectModel->GetEntitiesListSize(); i++)
    {
        const QModelIndex projectIndex = viewModel->index(0,0);

        bool isNew = projectModel->GetEntityAt(i)->IsNew();
        bool isChanged = projectModel->GetEntityAt(i)->isChanged();
        bool isRemoved = projectModel->GetEntityAt(i)->IsRemoved();

        if(isNew || isChanged)
        {
            // Ogni nuovo task/milestone lo aggiungo al suo posto dopo i task group
            if(isNew)
            {
                if ( !viewModel->insertRow( projectModel->GetTaskGroupListSize() + i, projectIndex ) )
                    return;
            }

            int row = projectModel->GetTaskGroupListSize() + i;
            if ( row == 0 && projectIndex.isValid() && isNew )
                viewModel->insertColumns( viewModel->columnCount( projectIndex ), 5, projectIndex );

            if(projectModel->GetEntityAt(i)->getEntityType() == Task_type)
            {
                viewModel->setData( viewModel->index( row, 0, projectIndex ), static_cast<Task *>(projectModel->GetEntityAt(i))->getName() );
                viewModel->setData( viewModel->index( row, 1, projectIndex ), KDGantt::TypeTask );
                // Devo settare il modello sia per la ganttView che per la TreeView
                viewModel->setData( viewModel->index( row, 2, projectIndex ), static_cast<Task *>(projectModel->GetEntityAt(i))->getStart(), KDGantt::StartTimeRole );
                viewModel->setData( viewModel->index( row, 2, projectIndex ), static_cast<Task *>(projectModel->GetEntityAt(i))->getStart().toString("dd/MM/yyyy hh:mm") );
                viewModel->setData( viewModel->index( row, 3, projectIndex ), static_cast<Task *>(projectModel->GetEntityAt(i))->getEnd(), KDGantt::EndTimeRole );
                viewModel->setData( viewModel->index( row, 3, projectIndex ), static_cast<Task *>(projectModel->GetEntityAt(i))->getEnd().toString("dd/MM/yyyy hh:mm") );
                viewModel->setData( viewModel->index( row, 4, projectIndex ), static_cast<Task *>(projectModel->GetEntityAt(i))->getCompletition() );
            }
            else if(projectModel->GetEntityAt(i)->getEntityType() == Milestone_type)
            {
                viewModel->setData( viewModel->index( row, 0, projectIndex ), static_cast<Milestone *>(projectModel->GetEntityAt(i))->getName() );
                viewModel->setData( viewModel->index( row, 1, projectIndex ), KDGantt::TypeEvent );
                // Devo settare il modello sia per la ganttView che per la TreeView
                viewModel->setData( viewModel->index( row, 2, projectIndex ), static_cast<Milestone *>(projectModel->GetEntityAt(i))->getDateTime(), KDGantt::StartTimeRole );
                viewModel->setData( viewModel->index( row, 2, projectIndex ), static_cast<Milestone *>(projectModel->GetEntityAt(i))->getDateTime().toString("dd/MM/yyyy hh:mm") );
            }

            viewModel->itemFromIndex(viewModel->index(row, 0, projectIndex))->setEditable(false);

#ifdef DRAGNDROP
            viewModel->itemFromIndex(viewModel->index(row, 0, projectIndex))->setDragEnabled(true);
            viewModel->itemFromIndex(viewModel->index(row, 0, projectIndex))->setDropEnabled(false);
#endif

            if(isNew)
            {
                QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
                leftView->expand(projectIndex);
            }
        }
        else if(isRemoved)
        {
            viewModel->removeRow(projectModel->GetTaskGroupListSize() + i, projectIndex);
            qobject_cast<QTreeView*>( ui->ganttView->leftView() )->clearSelection();
        }
    }
}

void MainWindow::EnableMenu()
{
    // Abilito alcune voci del menu
    ui->action_Save_as->setEnabled(true);
    ui->action_Close_Project->setEnabled(true);
    ui->actionAdd_Task_Group->setEnabled(true);
    ui->actionAdd_Task->setEnabled(true);
    ui->actionAdd_Milestone->setEnabled(true);
    ui->action_Edit_Project->setEnabled(true);
    ui->action_Edit_Task_Group->setEnabled(true);
    ui->action_Edit_Task->setEnabled(true);
    ui->action_Edit_Milestone->setEnabled(true);
}

void MainWindow::DisableMenu()
{
    ui->action_Save_as->setEnabled(false);
    ui->action_Close_Project->setEnabled(false);
    ui->actionAdd_Task_Group->setEnabled(false);
    ui->actionAdd_Task->setEnabled(false);
    ui->actionAdd_Milestone->setEnabled(false);
    ui->action_Edit_Project->setEnabled(false);
    ui->action_Edit_Task_Group->setEnabled(false);
    ui->action_Edit_Task->setEnabled(false);
    ui->action_Edit_Milestone->setEnabled(false);
}

void MainWindow::on_actionNew_Project_triggered()
{
    if(!bEmptyProject)
    {
        // Controllo se ho eliminato il progetto
        QMessageBox::StandardButton result = QMessageBox::information(this,
                                                          "Warning",
                                                          "Do you want to save the project?",
                                                          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                                          QMessageBox::Cancel);

        if(result == QMessageBox::Cancel)
        {
            return;
        }
        else if(result == QMessageBox::No)
        {
            DeleteProject();
        }
        else if(result == QMessageBox::Yes)
        {
            // Fare in modo che se scelgo di salvare e poi premo cancel nella
            // dialog di salvataggio si annulla tutto (adesso invece non salvo il proj
            // e lo cancello)
            on_action_Save_as_triggered();
            DeleteProject();
        }
    }

    AddProjectDialog *dialog = new AddProjectDialog( this );
    if ( dialog->exec() == QDialog::Rejected || !dialog ) {
        delete dialog;
        return;
    }

    TeeamProject *newProject = new TeeamProject(dialog->GetProjectName(), dialog->GetPeopleList());
    this->projectModel = newProject;
    newProject->attach(this);
    ganttController->NewProject(newProject);
    bEmptyProject = false;

    // Abilito alcune voci del menu 
    EnableMenu();

    delete dialog;
    return;
}

void MainWindow::on_actionAdd_Task_Group_triggered()
{
    QList<QString> existingGroups;
    for(int i = 0; i < projectModel->GetTaskGroupListSize(); i++)
        existingGroups.append(projectModel->GetTaskGroupAt(i)->getName());

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
    for(int i = 0; i < projectModel->GetTaskGroupListSize(); i++)
        groupList << projectModel->GetTaskGroupAt(i)->getName();

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

    ganttController->AddTask(this, taskName, start, end, taskPeople, completition, selectedParent);
    
	delete dialog;
    return;
}

void MainWindow::on_actionAdd_Milestone_triggered()
{
    QList<QString> groupList;
    for(int i = 0; i < projectModel->GetTaskGroupListSize(); i++)
        groupList << projectModel->GetTaskGroupAt(i)->getName();

    QList<QString> totalPeople = projectModel->GetPeopleList();

    AddMilestoneDialog *dialog = new AddMilestoneDialog( groupList, totalPeople, this );
    if ( dialog->exec() == QDialog::Rejected || !dialog ) {
        delete dialog;
        return;
    }

    QString milestoneName = dialog->GetMilestoneName();
    int selectedParent = dialog->GetSelectedGroup();
    QDateTime start = dialog->GetStartDateTime();
    QList<QString> milestonePeople = dialog->GetPeople();

    ganttController->AddMilestone(this, milestoneName, start, milestonePeople, selectedParent);
    
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

    EditEntityAtIndex(index);
}

void MainWindow::on_actionTreeView_rightclick(const QPoint &point)
{
    QModelIndex index = ui->ganttView->leftView()->indexAt(point);
    if(index.isValid())
    {
        QMenu menu;
        QString editText = "Edit";
        QString removeText = "Remove";
        menu.addAction(editText);
        menu.addAction(removeText);

        QAction* selectedVoice = menu.exec(ui->ganttView->mapToGlobal(point));
        if(selectedVoice)
        {
            if( selectedVoice->text() == editText )
            {
                EditEntityAtIndex(index);
            }
            else if( selectedVoice->text() == removeText )
            {
                RemoveEntityFromIndex(index);
            }
            else
            {
                return;
            }
        }
        else
        {
            return;
        }
    }
}


void MainWindow::on_actionTreeView_del(const QModelIndex &index)
{
    RemoveEntityFromIndex(index);
}

void MainWindow::EditEntityAtIndex(const QModelIndex index)
{
    if(index.row() == 0 && index.column() == 0 && !index.parent().isValid())
    {
        // Ho cliccato il project
        on_action_Edit_Project_triggered();
    }
    else if(index.parent().isValid() && !index.parent().parent().isValid())
    {
        if(index.row() < projectModel->GetTaskGroupListSize())
        {
             // Controllo se ho cliccato un gruppo
            QList<QString> groups;
            for (int i = 0; i < projectModel->GetTaskGroupListSize(); i++)
                groups << projectModel->GetTaskGroupAt(i)->getName();

            EditTaskGroupDialog *dialog = new EditTaskGroupDialog( groups, this, index.row() );
            if ( dialog->exec() == QDialog::Rejected || !dialog ) {
                delete dialog;
                return;
            }

            QString newName = dialog->GetTaskGroupName();

            if(newName != projectModel->GetTaskGroupAt(index.row())->getName())
            {
                ganttController->EditTaskGroup(index.row(), newName);
            }

            delete dialog;
            return;
        }
        else
        {
            // oppure se ho cliccato un task/milestone non dipendente da nessun gruppo
            int entityIndex = index.row() - projectModel->GetTaskGroupListSize();
            QList<QString> groups;
            for (int i = 0; i < projectModel->GetTaskGroupListSize(); i++)
                groups << projectModel->GetTaskGroupAt(i)->getName();

            if(projectModel->GetEntityAt(entityIndex)->getEntityType() == Task_type)
            {
                EditTaskDialog *dialog = new EditTaskDialog( static_cast<Task*>(projectModel->GetEntityAt(entityIndex))->getName(),
                                                             -1,
                                                             groups,
                                                             static_cast<Task*>(projectModel->GetEntityAt(entityIndex))->getPeople(),
                                                             projectModel->GetPeopleList(),
                                                             static_cast<Task*>(projectModel->GetEntityAt(entityIndex))->getStart(),
                                                             static_cast<Task*>(projectModel->GetEntityAt(entityIndex))->getEnd(),
                                                             static_cast<Task*>(projectModel->GetEntityAt(entityIndex))->getCompletition(),
                                                             this);

                if ( dialog->exec() == QDialog::Rejected || !dialog ) {
                    delete dialog;
                    return;
                }

                ganttController->EditTaskOrMilestone(this,
                                                     dialog->GetTaskName(),
                                                     dialog->GetStartDateTime(),
                                                     dialog->GetEndDateTime(),
                                                     dialog->GetPeople(),
                                                     dialog->GetCompletition(),
                                                     dialog->GetSelectedGroup(),
                                                     entityIndex);
            }
            else if(projectModel->GetEntityAt(entityIndex)->getEntityType() == Milestone_type)
            {
                EditMilestoneDialog *dialog = new EditMilestoneDialog( static_cast<Milestone*>(projectModel->GetEntityAt(entityIndex))->getName(),
                                                                       -1,
                                                                       groups,
                                                                       static_cast<Milestone*>(projectModel->GetEntityAt(entityIndex))->getPeople(),
                                                                       projectModel->GetPeopleList(),
                                                                       static_cast<Milestone*>(projectModel->GetEntityAt(entityIndex))->getDateTime(),
                                                                       this);

                if ( dialog->exec() == QDialog::Rejected || !dialog ) {
                    delete dialog;
                    return;
                }

                ganttController->EditTaskOrMilestone(this,
                                                     dialog->GetMilestoneName(),
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
        for (int i = 0; i < projectModel->GetTaskGroupListSize(); i++)
            groups << projectModel->GetTaskGroupAt(i)->getName();

        if(projectModel->GetTaskGroupAt(parentIndex)->GetEntityAt(entityIndex)->getEntityType() == Task_type)
        {
            EditTaskDialog *dialog = new EditTaskDialog( static_cast<Task*>(projectModel->GetTaskGroupAt(parentIndex)->GetEntityAt(entityIndex))->getName(),
                                                         parentIndex,
                                                         groups,
                                                         static_cast<Task*>(projectModel->GetTaskGroupAt(parentIndex)->GetEntityAt(entityIndex))->getPeople(),
                                                         projectModel->GetPeopleList(),
                                                         static_cast<Task*>(projectModel->GetTaskGroupAt(parentIndex)->GetEntityAt(entityIndex))->getStart(),
                                                         static_cast<Task*>(projectModel->GetTaskGroupAt(parentIndex)->GetEntityAt(entityIndex))->getEnd(),
                                                         static_cast<Task*>(projectModel->GetTaskGroupAt(parentIndex)->GetEntityAt(entityIndex))->getCompletition(),
                                                         this);

            if ( dialog->exec() == QDialog::Rejected || !dialog ) {
                delete dialog;
                return;
            }

            ganttController->EditTaskOrMilestone(this,
                                                 dialog->GetTaskName(),
                                                 dialog->GetStartDateTime(),
                                                 dialog->GetEndDateTime(),
                                                 dialog->GetPeople(),
                                                 dialog->GetCompletition(),
                                                 dialog->GetSelectedGroup(),
                                                 entityIndex,
                                                 parentIndex);
        }
        else if(projectModel->GetTaskGroupAt(parentIndex)->GetEntityAt(entityIndex)->getEntityType() == Milestone_type)
        {
            EditMilestoneDialog *dialog = new EditMilestoneDialog( static_cast<Milestone*>(projectModel->GetTaskGroupAt(parentIndex)->GetEntityAt(entityIndex))->getName(),
                                                                   parentIndex,
                                                                   groups,
                                                                   static_cast<Milestone*>(projectModel->GetTaskGroupAt(parentIndex)->GetEntityAt(entityIndex))->getPeople(),
                                                                   projectModel->GetPeopleList(),
                                                                   static_cast<Milestone*>(projectModel->GetTaskGroupAt(parentIndex)->GetEntityAt(entityIndex))->getDateTime(),
                                                                   this);

            if ( dialog->exec() == QDialog::Rejected || !dialog ) {
                delete dialog;
                return;
            }

            ganttController->EditTaskOrMilestone(this,
                                                 dialog->GetMilestoneName(),
                                                 dialog->GetStartDateTime(),
                                                 dialog->GetPeople(),
                                                 dialog->GetSelectedGroup(),
                                                 entityIndex,
                                                 parentIndex);
        }
    }
}

void MainWindow::RemoveEntityFromIndex(const QModelIndex index)
{
    if(index.isValid())
    {
        QString text = "Deleted row: " + QString::number(index.row()) + "; column: " + QString::number(index.column());
        qDebug() << text;

        if(index.row() == 0 && index.column() == 0 && !index.parent().isValid())
        {
            // Controllo se ho eliminato il progetto
            on_action_Close_Project_triggered();
        }
        else if(index.parent().isValid() && !index.parent().parent().isValid())
        {
            // oppure se ho eliminato un gruppo
            if(index.row() < projectModel->GetTaskGroupListSize())
            {
                ganttController->RemoveTaskGroup(index.row());
            }
            // oppure se ho eliminato un task/milestone non dipendente da nessun altro
            else
            {
                ganttController->RemoveTaskOrMilestone(index.row() - projectModel->GetTaskGroupListSize());
            }
        }
        else if(index.parent().parent().isValid() && !index.parent().parent().parent().isValid())
        {
            // oppure se ho eliminato un task/milestone sotto un gruppo
            ganttController->RemoveTaskOrMilestone(index.row(), index.parent().row());
        }
    }
}

void MainWindow::DeleteProject()
{
    // TODO : refactor
    QStringList list;
    TeeamProject *newProject = new TeeamProject("", list);
    if(projectModel != nullptr)
        delete projectModel;
    this->projectModel = newProject;
    newProject->attach(this);
    ganttController->NewProject(newProject);

    viewModel = new QStandardItemModel( 0, 6, this );
    viewModel->setHeaderData( 0, Qt::Horizontal, tr( "Project Tree View" ) );
    ui->ganttView->setModel( viewModel );

    bEmptyProject = true;
    DisableMenu();
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

    on_action_Close_Project_triggered();

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
    if(!bEmptyProject)
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
    if(!bEmptyProject)
    {
        QList<QString> groups;
        for (int i = 0; i < projectModel->GetTaskGroupListSize(); i++)
            groups << projectModel->GetTaskGroupAt(i)->getName();

        EditTaskGroupDialog *dialog = new EditTaskGroupDialog( groups, this );
        if ( dialog->exec() == QDialog::Rejected || !dialog ) {
            delete dialog;
            return;
        }

        QString newName = dialog->GetTaskGroupName();
        int selectedGroup = dialog->GetSelectedGroup();

        if(newName != projectModel->GetTaskGroupAt(selectedGroup)->getName())
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

    setCursor(Qt::WaitCursor);
    if(filename != "")
    {
        ganttController->SaveProjectAs(filename);
    }
    setCursor(Qt::ArrowCursor);
}

void MainWindow::on_actionOpen_File_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                       tr("Open Teeam Project"), ".",
                                       tr("Teeam files (*.tmproj)"));

    setCursor(Qt::WaitCursor);
    bEmptyProject = false;
    if(ganttController->OpenFile(filename, this) != NO_ERROR)
    {
        DeleteProject();
        bEmptyProject = true;
        QMessageBox::warning(this, "Warning", "File not valid.");
    }
    else
    {
        // È terminato il caricamento
        EnableMenu();
    }

    setCursor(Qt::ArrowCursor);
    return;
}

void MainWindow::on_action_Close_Project_triggered()
{
    if(!bEmptyProject)
    {
        // Controllo se ho eliminato il progetto
        QMessageBox::StandardButton result = QMessageBox::information(this,
                                                          "Warning",
                                                          "Do you want to save the project?",
                                                          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                                          QMessageBox::Cancel);

        if(result == QMessageBox::Cancel)
            return;
        else if(result == QMessageBox::No)
        {
            DeleteProject();
            return;
        }
        else if(result == QMessageBox::Yes)
        {
            on_action_Save_as_triggered();
            DeleteProject();
            return;
        }
    }
    return;
}
