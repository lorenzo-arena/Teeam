#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addprojectdialog.h"

#include <QApplication>
#include <QList>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTreeView>
#include <QSettings>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initGanttView();

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

    if (settings.contains(KEY_MAINWINDOW_POS)) // setto la posizione della finestra
    {
        move(settings.value(KEY_MAINWINDOW_POS).toPoint());
    }
    settings.endGroup();

    ganttController = new GanttController();
}

MainWindow::~MainWindow()
{
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

void MainWindow::closeEvent(QCloseEvent *eventArgs)
{
    QSettings settings;
    settings.beginGroup(KEY_MAINWINDOW);
    settings.setValue(KEY_MAINWINDOW_MAXIMIZED, isMaximized());
    settings.setValue(KEY_MAINWINDOW_SIZE, size());
    settings.setValue(KEY_MAINWINDOW_POS, pos());
    settings.endGroup();
    eventArgs->accept();
}
