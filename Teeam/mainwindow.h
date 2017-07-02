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

    void on_actionAdd_Project_triggered();

    void on_actionAdd_Task_Group_triggered();

    void on_actionAdd_Task_triggered();

    void on_actionAdd_Milestone_triggered();

    void closeEvent(QCloseEvent *eventArgs);

// private methods
private:
    void initGanttView();

// private components
private:
    Ui::MainWindow* ui;
    KDGantt::DateTimeGrid* dateTimeGrid;
    GanttController* ganttController;
    const QString KEY_MAINWINDOW = "teeam_mainwindow";
    const QString KEY_MAINWINDOW_MAXIMIZED = "maximized";
    const QString KEY_MAINWINDOW_SIZE = "size";
    const QString KEY_MAINWINDOW_POS = "position";
};

#endif // MAINWINDOW_H
