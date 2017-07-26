#include "editmilestonedialog.h"
#include "ui_editmilestonedialog.h"

EditMilestoneDialog::EditMilestoneDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditMilestoneDialog)
{
    ui->setupUi(this);
}

EditMilestoneDialog::~EditMilestoneDialog()
{
    delete ui;
}
