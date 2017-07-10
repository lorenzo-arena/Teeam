#include "addmilestonedialog.h"
#include "ui_addmilestonedialog.h"

AddMilestoneDialog::AddMilestoneDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMilestoneDialog)
{
    ui->setupUi(this);
}

AddMilestoneDialog::~AddMilestoneDialog()
{
    delete ui;
}
