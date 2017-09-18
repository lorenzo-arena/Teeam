#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "abstractview.h"
#include "abstractmodel.h"
#include "ganttcontroller.h"
#include "freedaysmodel.h"
#include "teeamproject.h"

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMessageBox>

#include <KDGanttDateTimeGrid>
#include <KDGanttConstraintModel>
#include <KDGanttDateTimeScaleFormatter>
#include <KDGanttView>
#include <KDGanttGraphicsView>

namespace Ui {
class MainWindow;
}

#ifdef DATETIMESCALE_MOD
class TeeamDateTimeScaleFormatter : public KDGantt::DateTimeScaleFormatter
{
public:
    explicit TeeamDateTimeScaleFormatter( const DateTimeScaleFormatter& other );

private:
    QString text( const QDateTime& datetime );
};
#endif

class MainWindow : public QMainWindow, public AbstractView
{
    Q_OBJECT

public:
    explicit MainWindow(GanttController *ganttController, FreeDaysModel *freeDaysModel, QString appVersion, QString projectPath = "", QWidget *parent = nullptr);
    ~MainWindow();

    void UpdateView();
    void SetProjectModel(TeeamProject *project) { this->projectModel = project; }

    void DisableMenu();

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
    void on_actionTreeView_rightclick(const QPoint& point);
    void on_actionTreeView_del(const QModelIndex &index);
    void on_action_Edit_Project_triggered();
    void on_action_Edit_Task_Group_triggered();
    void on_action_Edit_Task_triggered();
    void on_action_Edit_Milestone_triggered();
    void on_action_Save_as_triggered();
    void on_actionOpen_File_triggered();
    void on_action_Close_Project_triggered();
    bool eventFilter(QObject *target, QEvent *event);
    void closeEvent(QCloseEvent *eventArgs);

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
    bool bEmptyProject;

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

    void updateFreeDaysView();
    void updateProjectView();
    void updateTaskGroupView();
    void updateEntitiesView();
    void deleteProject();
    void enableMenu();
    void editEntityAtIndex(const QModelIndex index);
    void removeEntityFromIndex(const QModelIndex index);
    bool projectSaveAs();
};

#endif // MAINWINDOW_H
