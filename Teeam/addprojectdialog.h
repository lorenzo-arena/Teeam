#ifndef ADDPROJECTDIALOG_H
#define ADDPROJECTDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QKeyEvent>
#include <QStringListModel>
#include <QList>
#include <QString>

namespace Ui {
    class AddProjectDialog;
}

class AddProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddProjectDialog(QWidget *parent = 0, Qt::WindowFlags f = 0 );
    explicit AddProjectDialog(QString name, QList<QString> people, QWidget *parent = 0, Qt::WindowFlags f = 0 );

    QString GetProjectName() { return projectName; }
    QList<QString> GetPeopleList() { return peopleModel.stringList(); }

private slots:
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();
    bool eventFilter(QObject *target, QEvent *event);
    void on_addPersonButton_clicked();
    void on_removePersonButton_clicked();

private:
    Ui::AddProjectDialog* ui;
    QString projectName;
    QStringListModel peopleModel;
};

#endif // ADDPROJECTDIALOG_H
