#include "edittaskdialog.h"
#include "ui_edittaskdialog.h"

EditTaskDialog::EditTaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTaskDialog)
{
    ui->setupUi(this);
}

EditTaskDialog::~EditTaskDialog()
{
    delete ui;
}
