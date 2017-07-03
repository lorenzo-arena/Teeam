#include "freedaysdialog.h"
#include "ui_freedaysdialog.h"

FreeDaysDialog::FreeDaysDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FreeDaysDialog)
{
    ui->setupUi(this);
}

FreeDaysDialog::~FreeDaysDialog()
{
    delete ui;
}

// TODO : aggiungere metodi per gestione salvataggio giorni liberi
