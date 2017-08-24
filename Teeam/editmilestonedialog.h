#ifndef EDITMILESTONEDIALOG_H
#define EDITMILESTONEDIALOG_H

#include "teeamproject.h"
#include "ganttentity.h"
#include "generictask.h"
#include "taskgroup.h"
#include "milestone.h"

#include <QDialog>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QStringListModel>

namespace Ui {
class EditMilestoneDialog;
}

class EditMilestoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditMilestoneDialog(QWidget *parent = 0);
    explicit EditMilestoneDialog(QString milestoneName, int group, QList<QString> milestoneGroups, QList<QString> milestonePeople, QList<QString> totalPeople, QDateTime start, QWidget *parent = 0);
    ~EditMilestoneDialog();
    int GetSelectedGroup() { return selectedGroup - 1; } // Devo restituire il numero senza la voce "None"
    QString GetMilestoneName() { return name; }
    QDateTime GetStartDateTime() { return start; }
    QList<QString> GetPeople() { return milestonePeopleModel.stringList(); }
    //TeeamProject GetEdited() { return project; }

private slots:
    bool eventFilter(QObject *target, QEvent *event);
    void on_baseGroupcomboBox_currentIndexChanged(int index);
    void on_baseMilestonecomboBox_currentIndexChanged(int index);
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();
    void on_insertPeoplepushButton_clicked();
    void on_removePeoplepushButton_clicked();
    void on_startdateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
    void on_milestoneGroupcomboBox_currentIndexChanged(int index);

private:
    Ui::EditMilestoneDialog *ui;
    QString name;
    int selectedGroup;
    QDateTime start;
    QStringListModel totalPeopleModel;
    QStringListModel milestonePeopleModel;
    //TeeamProject project; // Inserire costruttore copia!
    bool singleEditMode;
    bool multipleEditMode;

    void DisablePanel();
    void EnablePanel();
};

#endif // EDITMILESTONEDIALOG_H
