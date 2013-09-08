#include "settings.h"
#include "ui_settings.h"
#include "trdialog.h"

#include <QSettings>
#include <QMessageBox>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings),
    m_trDlg(new TrDialog(this))
{
    ui->setupUi(this);

    foreach (TrBackend backend, backends()) {
        QString label = backend["host"].toString();
        if (backend["is_default"].toBool())
            label += " (Default)";
        ui->listWidget->addItem(label);
    }

    int act = defaultAction();
    if (act == 0) {
        ui->copyLinkBtn->setChecked(true);
    } else if (act == 1) {
        ui->addTrBtn->setChecked(true);
    }
}

Settings::~Settings()
{
    delete ui;
}

TrBackend Settings::defaultBackend()
{
    TrBackend fallback;
    QList<TrBackend> trBackends = backends();
    QSettings s;

    foreach (TrBackend backend, trBackends) {
        if (backend["is_default"].toBool())
            return backend;
    }

    // no default found
    if (trBackends.size() > 0) {
        trBackends[0]["is_default"] = true;
        return trBackends[0];
    } else {
        return fallback;
    }
}

QList<TrBackend> Settings::backends()
{
    QList<TrBackend> backends;
    QSettings s;
    foreach (QVariant b, s.value("backends").toList()) {
        backends << b.toMap();
    }
    return backends;
}

int Settings::defaultAction()
{
    QSettings s;
    return s.value("defaultAction").toInt();
}

void Settings::setBackends(QList<TrBackend> backends)
{
    QSettings s;
    QList<QVariant> variant;
    foreach(TrBackend b, backends) {
        variant << QVariant::fromValue(b);
    }
    s.setValue("backends", variant);
}

void Settings::on_buttonBox2_accepted()
{
    QSettings s;
    int which = 0;
    if (ui->addTrBtn->isChecked()) {
        which = 1;
    }
    s.setValue("defaultAction", which);
}

void Settings::on_addBtn_clicked()
{
    m_trDlg->reset();
    if (m_trDlg->exec() != QDialog::Accepted)
        return;

    QList<TrBackend> trBackends = backends();
    TrBackend backend = m_trDlg->getBackend();
    backend["is_default"] = trBackends.empty();
    trBackends << backend;

    setBackends(trBackends);

    QString label = backend["host"].toString();
    if (backend["is_default"].toBool())
        label += " (Default)";
    ui->listWidget->addItem(label);
}

void Settings::on_removeBtn_clicked()
{
    if (ui->listWidget->selectedItems().size() == 0) {
        QMessageBox::warning(this, "Error", "No server selected");
        return;
    }

    int current = ui->listWidget->currentRow();
    QList<TrBackend> trBackends = backends();
    if (trBackends[current]["is_default"].toBool()) {
        for (int i=0; i<trBackends.size(); i++) {
            if (i != current) {
                trBackends[i]["is_default"] = true;
                QListWidgetItem *item = ui->listWidget->item(i);
                item->setText(item->text()+" (Default)");
                break;
            }
        }
    }
    trBackends.takeAt(current);
    setBackends(trBackends);
    ui->listWidget->takeItem(current);
}

void Settings::on_defaultBtn_clicked()
{
    QList<TrBackend> trBackends = backends();
    for (int i=0; i<ui->listWidget->count(); i++) {
        bool is_default = ui->listWidget->currentRow()==i;
        trBackends[i]["is_default"] = is_default;
        QString label = trBackends[i]["host"].toString();
        if (is_default)
            label += " (Default)";
        ui->listWidget->item(i)->setText(label);
    }
    setBackends(trBackends);
}

void Settings::on_editBtn_clicked()
{
    if (ui->listWidget->selectedItems().size() == 0) {
        QMessageBox::warning(this, "Error", "No server selected");
        return;
    }

    int current = ui->listWidget->currentRow();

    QList<TrBackend> trBackends = backends();
    m_trDlg->loadBackend(trBackends[current]);

    if (m_trDlg->exec() != QDialog::Accepted)
        return;

    trBackends[current] = m_trDlg->getBackend();
    setBackends(trBackends);
}
