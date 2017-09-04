#ifndef ADDMILESTONEDIALOG_H
#define ADDMILESTONEDIALOG_H

#include <QDialog>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QStringListModel>

namespace Ui {
class AddMilestoneDialog;
}

class AddMilestoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMilestoneDialog(QList<QString> groupList, QList<QString> peopleList, QWidget *parent = 0);
    ~AddMilestoneDialog();
    int GetSelectedGroup() { return selectedGroup - 1; } // L'indice è zero-based
    QString GetMilestoneName() { return name; }
    QDateTime GetStartDateTime() { return start; }
    QList<QString> GetPeople() { return milestonePeopleModel.stringList(); }

private:
    Ui::AddMilestoneDialog *ui;
    QString name;
    int selectedGroup;
    QDateTime start;
    QStringListModel totalPeopleModel;
    QStringListModel milestonePeopleModel;

private slots:
    bool eventFilter(QObject *target, QEvent *event);
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();
    void on_startdateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
    void on_insertPeoplepushButton_clicked();
    void on_removePeoplepushButton_clicked();
    void on_milestoneGroupcomboBox_currentIndexChanged(int index);
};

#endif // ADDMILESTONEDIALOG_H
