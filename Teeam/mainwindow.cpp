#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addprojectdialog.h"

#include <QApplication>
#include <QList>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTreeView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initGanttView();

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
    model->setHeaderData( 0, Qt::Horizontal, tr( "Tree View of Entries" ) );
    ui->ganttView->setModel( model );

    QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
    leftView->setColumnHidden( 1, true );
    leftView->header()->setStretchLastSection( true );
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
    AddProjectDialog* dialog = new AddProjectDialog( this );
    if ( dialog->exec() == QDialog::Rejected || !dialog ) {
        delete dialog;
        return;
    }

    QString projectName = dialog->GetProjectName();

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
