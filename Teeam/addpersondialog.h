#ifndef ADDPERSONDIALOG_H
#define ADDPERSONDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class AddPersonDialog;
}

class AddPersonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPersonDialog(QWidget *parent = 0, Qt::WindowFlags f = 0 );
    ~AddPersonDialog();

    QString GetPersonName() { return personName; }

private slots:
    bool eventFilter(QObject *target, QEvent *event);

    void on_okbutton_clicked();

    void on_cancelbutton_clicked();

private:
    Ui::AddPersonDialog *ui;
    QString personName;
};

#endif // ADDPERSONDIALOG_H
