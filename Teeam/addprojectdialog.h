#ifndef ADDPROJECTDIALOG_H
#define ADDPROJECTDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QKeyEvent>

namespace Ui {
    class AddProjectDialog;
}

class AddProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddProjectDialog(QWidget *parent = 0, Qt::WindowFlags f = 0 );

    QString GetProjectName();

private slots:
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();
    bool eventFilter(QObject *target, QEvent *event);

private:
    Ui::AddProjectDialog* ui;
    QString projectName;
};

#endif // ADDPROJECTDIALOG_H
