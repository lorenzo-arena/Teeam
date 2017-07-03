#include "freedaysdialog.h"
#include "ui_freedaysdialog.h"

#include <QColorDialog>
#include <QBrush>

FreeDaysDialog::FreeDaysDialog(Days days, QColor color, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FreeDaysDialog)
{
    ui->setupUi(this);

    QDialog::setFixedSize(this->size());

    if(days.bMonday)
        ui->checkBox_mon->setChecked(true);
    if(days.bTuesday)
        ui->checkBox_tue->setChecked(true);
    if(days.bWednesday)
        ui->checkBox_wed->setChecked(true);
    if(days.bThursday)
        ui->checkBox_thu->setChecked(true);
    if(days.bFriday)
        ui->checkBox_fri->setChecked(true);
    if(days.bSaturday)
        ui->checkBox_sat->setChecked(true);
    if(days.bSunday)
        ui->checkBox_sun->setChecked(true);

    QPixmap pixmap(ui->ButtonColor->width(),ui->ButtonColor->height());
    pixmap.fill(color);
    QIcon ButtonIcon(pixmap);
    ui->ButtonColor->setIcon(pixmap);
    this->color = color;
}

FreeDaysDialog::~FreeDaysDialog()
{
    delete ui;
}

Days FreeDaysDialog::GetSelectedDays()
{
    return days;
}

QColor FreeDaysDialog::GetSelectedColor()
{
    return color;
}

void FreeDaysDialog::on_buttonOK_clicked()
{
    days = {
        ui->checkBox_mon->isChecked(),
        ui->checkBox_tue->isChecked(),
        ui->checkBox_wed->isChecked(),
        ui->checkBox_thu->isChecked(),
        ui->checkBox_fri->isChecked(),
        ui->checkBox_sat->isChecked(),
        ui->checkBox_sun->isChecked(),
    };
    QDialog::accept();
}

void FreeDaysDialog::on_buttonCancel_clicked()
{
    QDialog::reject();
}

void FreeDaysDialog::on_ButtonColor_clicked()
{
    QColor tempColor = QColorDialog::getColor(color, this);
    if(!tempColor.isValid()) return;
    QPixmap pixmap(ui->ButtonColor->width(),ui->ButtonColor->height());
    pixmap.fill(tempColor);
    QIcon ButtonIcon(pixmap);
    ui->ButtonColor->setIcon(pixmap);
    color = tempColor;
}
