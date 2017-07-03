#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addprojectdialog.h"
#include "freedaysdialog.h"

#include <QApplication>
#include <QList>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTreeView>
#include <QSettings>
#include <QCloseEvent>
#include <QSet>

MainWindow::MainWindow(FreeDaysModel *freeDaysModel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initGanttView();

    // Aggancio i vari modelli
    this->freeDaysModel = freeDaysModel;
    this->freeDaysModel->attach(this);

    ganttController = new GanttController(freeDaysModel);

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
    //--------------------------------------------------------
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

    //------------------------------------------------------------
}

MainWindow::~MainWindow()
{
    freeDaysModel->detach(this);
    delete ganttController;
    delete dateTimeGrid;
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
    dateTimeGrid->setDayWidth(50);
    ui->ganttView->setGrid( dateTimeGrid );

    QStandardItemModel *model = new QStandardItemModel( 0, 1, this );
    model->setHeaderData( 0, Qt::Horizontal, tr( "Project Tree View" ) );
    ui->ganttView->setModel( model );

    QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
    leftView->setColumnHidden( 1, true );
    leftView->header()->setStretchLastSection( true );

    ui->ganttView->installEventFilter(this);
}

void MainWindow::UpdateView()
{
    if(freeDaysModel->GetChanged())
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
    return;
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

    QString projectName = dialog->GetProjectName();
    delete dialog;
    return;
}

void MainWindow::on_actionAdd_Task_Group_triggered()
{

}

void MainWindow::on_actionAdd_Task_triggered()
{

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
    else
        dateTimeGrid->setDayWidth(dayWidth + 20);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    qreal dayWidth = dateTimeGrid->dayWidth();
    if (dayWidth <= 400)
    {
        dateTimeGrid->setScale(KDGantt::DateTimeGrid::ScaleDay);
        if (dayWidth <= 20)
             dateTimeGrid->setDayWidth(20);
        else
            dateTimeGrid->setDayWidth(dayWidth - 20);
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


