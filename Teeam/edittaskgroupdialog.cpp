#include "edittaskgroupdialog.h"
#include "ui_edittaskgroupdialog.h"

EditTaskGroupDialog::EditTaskGroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTaskGroupDialog)
{
    ui->setupUi(this);
}

EditTaskGroupDialog::~EditTaskGroupDialog()
{
    delete ui;
}
