#include "trdialog.h"
#include "ui_trdialog.h"

TrDialog::TrDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrDialog)
{
    ui->setupUi(this);
}

TrDialog::~TrDialog()
{
    delete ui;
}

TrBackend TrDialog::getBackend()
{
    TrBackend b;
    b["host"] = ui->hostEdit->text();
    b["port"] = ui->portEdit->text();
    b["user"] = ui->userEdit->text();
    b["passwd"] = ui->passEdit->text();

    return b;
}

void TrDialog::loadBackend(TrBackend b)
{
    ui->hostEdit->setText(b["host"].toString());
    ui->portEdit->setText(b["port"].toString());

    QString user = b["user"].toString();
    ui->authBox->setChecked(!user.isEmpty());

    ui->userEdit->setText(user);
    ui->passEdit->setText(b["passwd"].toString());

    ui->hostEdit->setFocus();
}

void TrDialog::reset()
{
    ui->hostEdit->clear();
    ui->portEdit->setText("9091");
    ui->authBox->setChecked(false);
    ui->userEdit->clear();
    ui->passEdit->clear();
    ui->hostEdit->setFocus();
}
