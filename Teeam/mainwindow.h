#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "abstractview.h"
#include "ganttcontroller.h"

#include <QMainWindow>

#include <KDGanttDateTimeGrid>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public AbstractView
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void UpdateView();

private slots:
    void on_action_Quit_triggered();

// private components
    void on_actionAdd_Project_triggered();

    void on_actionAdd_Task_Group_triggered();

    void on_actionAdd_Task_triggered();

    void on_actionAdd_Milestone_triggered();

private:
    Ui::MainWindow* ui;
    KDGantt::DateTimeGrid* dateTimeGrid;
    GanttController* ganttController;


// private methods
private:
    void initGanttView();
};

#endif // MAINWINDOW_H
