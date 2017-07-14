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

class MyStandardItem : public QStandardItem {
public:
  MyStandardItem( const QVariant& v ) : QStandardItem()
  {
    setData( v, Qt::DisplayRole );
  }
  MyStandardItem( const QString& v ) : QStandardItem()
  {
    setData( v, Qt::DisplayRole );
  }
};

MainWindow::MainWindow(GanttController *ganttController, FreeDaysModel *freeDaysModel, TeeamProject *projectModel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Aggancio i vari modelli
    this->freeDaysModel = freeDaysModel;
    this->freeDaysModel->attach(this);

    this->projectModel = projectModel;
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

    dateTimeGrid = new KDGantt::DateTimeGrid();
    ui->ganttView->setGrid( dateTimeGrid );

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

    if(projectModel->isChanged())
    {
        if(projectModel->isProjectChanged())
        {
            UpdateProjectView();
        }

        if(projectModel->isTaskGroupChanged())
        {
            UpdateTaskGroupView();
        }

        // TODO : refactor??
        if(projectModel->isEntitiesListChanged())
        {
            UpdateEntitiesView();
        }
    }

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
    QDateTime startdt = QDateTime::currentDateTime();
    QDateTime enddt = startdt.addDays( 1 );

    // TODO : aggiungere codice per ripulire dal vecchio progetto eventualmente

    // Da usare solo per aggiungere task o taskGroup
    //QModelIndexList selectedIndexes = ui->ganttView->selectionModel()->selectedIndexes();
    if ( !viewModel->insertRow( 0 ) )
        return;

    //viewModel->insertColumns( viewModel->columnCount(), 5 );

    viewModel->setData( viewModel->index( 0, 0 ), projectModel->getName() );
    viewModel->setData( viewModel->index( 0, 1 ), KDGantt::TypeSummary );
    //viewModel->setData( viewModel->index( 0, 2 ), startdt, KDGantt::StartTimeRole );
    //viewModel->setData( viewModel->index( 0, 3 ), enddt, KDGantt::EndTimeRole );
    viewModel->setData( viewModel->index( 0, 4 ), 10 );
    const QString legend( "" );
    if ( ! legend.isEmpty() )
        viewModel->setData( viewModel->index( 0, 5 ), legend );
}

void MainWindow::UpdateTaskGroupView()
{
    for(int i = 0; i < projectModel->GetTaskGroup().length(); i++)
    {
        if(projectModel->GetTaskGroup().at(i)->IsNew())
        {
            const QModelIndex parent = viewModel->index(0,0);

            if ( !viewModel->insertRow( i, parent ) )
                return;

            int row = i;
            if ( row == 0 && parent.isValid() )
                viewModel->insertColumns( viewModel->columnCount( parent ), 5, parent );

            viewModel->setData( viewModel->index( row, 0, parent ), projectModel->GetTaskGroup().at(i)->getName() );
            viewModel->setData( viewModel->index( row, 1, parent ), KDGantt::TypeSummary );
            //viewModel->setData( viewModel->index( row, 2, parent ), startdt, KDGantt::StartTimeRole );
            //viewModel->setData( viewModel->index( row, 3, parent ), enddt, KDGantt::EndTimeRole );
            viewModel->setData( viewModel->index( row, 4, parent ), 10 );
            const QString legend( "" );
            if ( ! legend.isEmpty() )
                viewModel->setData( viewModel->index( row, 5, parent ), legend );
        }
        else if(projectModel->GetTaskGroup().at(i)->isChanged())
        {
            // TODO : da utilizzare!
            /*const QModelIndex parent = viewModel->index(0,0);
            int row = i;
            viewModel->setData( viewModel->index( row, 0, parent ), projectModel->GetTaskGroup().at(i)->getName() );
            viewModel->setData( viewModel->index( row, 1, parent ), KDGantt::TypeSummary );
            //viewModel->setData( viewModel->index( row, 2, parent ), startdt, KDGantt::StartTimeRole );
            //viewModel->setData( viewModel->index( row, 3, parent ), enddt, KDGantt::EndTimeRole );
            viewModel->setData( viewModel->index( row, 4, parent ), 10 );
            const QString legend( "" );
            if ( ! legend.isEmpty() )
                viewModel->setData( viewModel->index( row, 5, parent ), legend );*/
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

            if ( !viewModel->insertRow( i, parent ) )
                return;

            int row = i;
            if ( row == 0 && parent.isValid() )
                viewModel->insertColumns( viewModel->columnCount( parent ), 5, parent );


            if(projectModel->GetEntitiesList().at(i)->getEntityType() == TASK_CODE)
            {
                viewModel->setData( viewModel->index( row, 0, parent ), static_cast<Task *>(projectModel->GetEntitiesList().at(i))->getName() );
                viewModel->setData( viewModel->index( row, 1, parent ), KDGantt::TypeTask );
                viewModel->setData( viewModel->index( row, 2, parent ), static_cast<Task *>(projectModel->GetEntitiesList().at(i))->getStart(), KDGantt::StartTimeRole );
                viewModel->setData( viewModel->index( row, 3, parent ), static_cast<Task *>(projectModel->GetEntitiesList().at(i))->getEnd(), KDGantt::EndTimeRole );
                viewModel->setData( viewModel->index( row, 4, parent ), 0 ); // TODO : add completition
                const QString legend( "" );
                if ( ! legend.isEmpty() )
                    viewModel->setData( viewModel->index( row, 5, parent ), legend );
            }
            else if(projectModel->GetEntitiesList().at(i)->getEntityType() == MILESTONE_CODE)
            {
                // TODO : implement!
                /*viewModel->setData( viewModel->index( row, 0, parent ), static_cast<Milestone *>(projectModel->GetEntitiesList().at(i))->getName()  );
                viewModel->setData( viewModel->index( row, 1, parent ), KDGantt::TypeEvent );
                viewModel->setData( viewModel->index( row, 2, parent ), static_cast<Milestone *>(projectModel->GetEntitiesList().at(i))->getStart(), KDGantt::StartTimeRole );
                //viewModel->setData( viewModel->index( row, 3, parent ), enddt, KDGantt::EndTimeRole );
                viewModel->setData( viewModel->index( row, 4, parent ), 0 ); // TODO : add completition
                const QString legend( "" );
                if ( ! legend.isEmpty() )
                    viewModel->setData( viewModel->index( row, 5, parent ), legend );*/
            }
            else
                return;
        }
        else if(projectModel->GetEntitiesList().at(i)->isChanged())
        {
            // TODO : da utilizzare!
            /*viewModel->setData( viewModel->index( row, 0, parent ), projectModel->GetEntitiesList().at(i)-> );
            viewModel->setData( viewModel->index( row, 1, parent ), KDGantt::TypeTask );
            viewModel->setData( viewModel->index( row, 2, parent ), startdt, KDGantt::StartTimeRole );
            viewModel->setData( viewModel->index( row, 3, parent ), enddt, KDGantt::EndTimeRole );
            viewModel->setData( viewModel->index( row, 4, parent ), 0 ); // TODO : add completition
            const QString legend( "" );
            if ( ! legend.isEmpty() )
                viewModel->setData( viewModel->index( row, 5, parent ), legend );*/
        }
    }
}

void MainWindow::on_action_Quit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAdd_Project_triggered()
{
    AddProjectDialog *dialog = new AddProjectDialog( this );
    if ( dialog->exec() == QDialog::Rejected || !dialog ) {
        delete dialog;
        return;
    }

    // TODO : re-inizializzo il progetto, devo salvare quello vecchio e agganciarmi al nuovo
    projectModel = new TeeamProject();
    ganttController->NewProject(this, projectModel, dialog->GetProjectName());

    delete dialog;
    return;
}

void MainWindow::on_actionAdd_Task_Group_triggered()
{
    AddTaskGroupDialog *dialog = new AddTaskGroupDialog( this );
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

    QList<QString> totalPeople;
    // TODO : add people initialization

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

    /********************************************************
    // Usare una cosa simile ma con la treeView per ritrovare la posizione del group parent??
    QComboBox* combo = new QComboBox();
    combo->addItem("True", "True");
    combo->addItem("False", "False");
    combo->setCurrentIndex(combo->findData("False"));
    *************************************************/
    if(selectedParent > 0)
        ganttController->AddTask(this, taskName, start, end, taskPeople, selectedParent);
    else
        ganttController->AddTask(this, taskName, start, end, taskPeople);
    delete dialog;
    return;
}

void MainWindow::on_actionAdd_Milestone_triggered()
{

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


