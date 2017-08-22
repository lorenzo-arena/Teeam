#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "abstractview.h"
#include "abstractmodel.h"
#include "ganttcontroller.h"
#include "freedaysmodel.h"
#include "teeamproject.h"

#include <QMainWindow>
#include <QStandardItemModel>

#include <KDGanttDateTimeGrid>
#include <KDGanttConstraintModel>
#include <KDGanttDateTimeScaleFormatter>
#include <KDGanttView>
#include <KDGanttGraphicsView>

namespace Ui {
class MainWindow;
}

class TeeamDateTimeScaleFormatter : public KDGantt::DateTimeScaleFormatter
{
public:
    TeeamDateTimeScaleFormatter( const DateTimeScaleFormatter& other );

private:
    QString text( const QDateTime& datetime );
};

class MainWindow : public QMainWindow, public AbstractView
{
    Q_OBJECT

public:
    explicit MainWindow(GanttController *ganttController, FreeDaysModel *freeDaysModel, TeeamProject *projectModel = nullptr, QWidget *parent = nullptr);
    ~MainWindow();

    void UpdateView();
    void SetProjectModel(TeeamProject *project) { this->projectModel = project; }

private slots:
    void on_action_Quit_triggered();
    void on_actionNew_Project_triggered();
    void on_actionAdd_Task_Group_triggered();
    void on_actionAdd_Task_triggered();
    void on_actionAdd_Milestone_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionSet_Free_Days_triggered();
    void on_actionTreeView_doubleclick(const QModelIndex &index);
    void on_actionTreeView_del(const QModelIndex &index);
    void on_action_Edit_Project_triggered();
    void on_action_Edit_Task_Group_triggered();
    void on_action_Edit_Task_triggered();
    void on_action_Edit_Milestone_triggered();
    bool eventFilter(QObject *target, QEvent *event);
    void closeEvent(QCloseEvent *eventArgs);
    void on_action_Save_as_triggered();

    void on_actionOpen_File_triggered();

private:
    void initGanttView();

// private components
private:
    Ui::MainWindow *ui;
    KDGantt::DateTimeGrid *dateTimeGrid;
    GanttController *ganttController;
    FreeDaysModel *freeDaysModel;
    TeeamProject *projectModel;
    QStandardItemModel *viewModel; // the model used to control the real GanttView
    KDGantt::ConstraintModel *costraintModel;

    // identificativi per settings MainWindow
    const QString REG_KEY_MAINWINDOW = "teeam_mainwindow";
    const QString REG_KEY_MAINWINDOW_MAXIMIZED = "maximized";
    const QString REG_KEY_MAINWINDOW_SIZE = "size";

    // identificativi per settings DateTimeView
    const QString REG_KEY_DATETIMEVIEW = "teeam_datetimeview";
    const QString REG_KEY_DATETIMEVIEW_SCALE = "scale";
    const QString REG_KEY_DATETIMEVIEW_DAYWIDTH = "daywidth";
    const QString REG_KEY_DATETIMEVIEW_FREEDAYSGROUP = "freedays";
    const QString REG_KEY_DATETIMEVIEW_MON = "mon";
    const QString REG_KEY_DATETIMEVIEW_TUE = "tue";
    const QString REG_KEY_DATETIMEVIEW_WED = "wed";
    const QString REG_KEY_DATETIMEVIEW_THU = "thu";
    const QString REG_KEY_DATETIMEVIEW_FRI = "fri";
    const QString REG_KEY_DATETIMEVIEW_SAT = "sat";
    const QString REG_KEY_DATETIMEVIEW_SUN = "sun";
    const QString REG_KEY_DATETIMEVIEW_COLOR = "color";

    // identificativi per file salvataggio
    const QString KEY_PROJECT = "teeamproject";
    const QString KEY_PROJECTPARAMETERS = "projectparameters";
    const QString KEY_NAME = "name";
    const QString KEY_PERSON = "person";
    const QString KEY_GROUP = "group";
    const QString KEY_ENTITY = "entity";
    const QString KEY_ENTITYTYPE = "entitytype";
    const QString KEY_TASKTYPE = "task";
    const QString KEY_MILESTONETYPE = "milestone";
    const QString KEY_STARTDATETIME = "startdatetime";
    const QString KEY_ENDDATETIME = "enddatetime";
    const QString KEY_COMPLETITION = "completition";

    void UpdateFreeDaysView();
    void UpdateProjectView();
    void UpdateTaskGroupView();
    void UpdateEntitiesView();
};

#endif // MAINWINDOW_H
