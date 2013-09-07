#include "ztorrent.h"
#include "ui_ztorrent.h"
#include "searchengine.h"
#include "transmission.h"
#include "settings.h"

#include <QMessageBox>
#include <QClipboard>
#include <QMenu>

ZTorrent::ZTorrent(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ZTorrent),
    m_engine(new SearchEngine(this)),
    m_transmission(new Transmission(this)),
    m_settings(new Settings(this))
{
    ui->setupUi(this);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);

    connect(m_engine, SIGNAL(finished(QList<Torrent>)), this, SLOT(showResults(QList<Torrent>)));
    connect(m_engine, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleError(QNetworkReply::NetworkError)));
    connect(m_transmission, SIGNAL(finished(QString,QString)), this, SLOT(torrentAdded(QString,QString)));
}

ZTorrent::~ZTorrent()
{
    delete ui;
    delete m_engine;
    delete m_transmission;
}

void ZTorrent::on_lineEdit_returnPressed()
{
    ui->statusBar->showMessage("Searching...");
    m_engine->search(ui->lineEdit->text());
}

void ZTorrent::showResults(QList<Torrent> torrents)
{
    ui->treeWidget->clear();
    foreach(Torrent torrent, torrents) {
        QStringList cols;
        cols << torrent["title"] << torrent["size"] << torrent["seeds"]+"/"+torrent["peers"];
        QTreeWidgetItem* i = new QTreeWidgetItem(ui->treeWidget, cols);
        i->setData(0, Qt::UserRole, torrent["torcache"]);
    }
    ui->statusBar->showMessage("Search complete. Double click on an item to execute the default action");
}

void ZTorrent::handleError(QNetworkReply::NetworkError error)
{
    QString message;
    switch (error) {
    case QNetworkReply::ConnectionRefusedError:
        message = "Connection refused"; break;
    case QNetworkReply::RemoteHostClosedError:
    case QNetworkReply::TemporaryNetworkFailureError:
        message = "Connection dropped"; break;
    case QNetworkReply::HostNotFoundError:
        message = "Could not resolve torrentz.in"; break;
    case QNetworkReply::TimeoutError:
        message = "Connection timed out"; break;
    case QNetworkReply::ContentNotFoundError:
        message = "404 Not Found"; break;
    case QNetworkReply::UnknownNetworkError:
    case QNetworkReply::UnknownContentError:
    default:
        message = "Unknown error"; break;
    }
    QMessageBox::critical(this, "Error", message);
}

void ZTorrent::torrentAdded(QString result, QString name)
{
    if (result == "success") {
        QMessageBox::information(this, "Transmission", name+" added for download");
    } else {
        QMessageBox::critical(this, "Transmission", "Error: "+result);
    }
    ui->treeWidget->setEnabled(true);
}

void ZTorrent::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    int act = m_settings->defaultAction();

    if (act == 0) {
        copyToClipboard(item);
    } else if (act == 1) {
        addToTransmission(item, m_settings->defaultBackend());
    }
}

void ZTorrent::copyToClipboard(QTreeWidgetItem *item)
{
    QString torcache = item->data(0, Qt::UserRole).toString();
    QApplication::clipboard()->setText(torcache);
    ui->statusBar->showMessage("Link copied to clipboad", 5000);
}

void ZTorrent::addToTransmission(QTreeWidgetItem *item, TrBackend backend)
{
    ui->treeWidget->setEnabled(false);
    QString torcache = item->data(0, Qt::UserRole).toString();
    m_transmission->addTorrent(torcache, backend);
}

void ZTorrent::on_goBtn_clicked()
{
    on_lineEdit_returnPressed();
}

void ZTorrent::on_buttonBox_clicked(QAbstractButton *button)
{
    QDialogButtonBox::StandardButton btn = ui->buttonBox->standardButton(button);
    if (btn == QDialogButtonBox::Reset) {
        ui->lineEdit->clear();
        ui->treeWidget->clear();
        ui->statusBar->clearMessage();
        ui->lineEdit->setFocus();
    } else if (btn == QDialogButtonBox::Close) {
        close();
    }
}

void ZTorrent::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    /*QMenu m;
    m.addAction("Open in browser...");
    m.addSeparator();
    m.addAction("Copy .torrent link");
    m.addAction("Add to transmission");

    m.exec(ui->treeWidget->mapToGlobal(pos));*/
}

void ZTorrent::on_settingsBtn_clicked()
{
    m_settings->exec();
}
