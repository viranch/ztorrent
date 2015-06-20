#include "ztorrent.h"
#include "ui_ztorrent.h"
#include "searchengine.h"
#include "transmission.h"
#include "settings.h"

#include <QMessageBox>
#include <QClipboard>
#include <QMenu>
#include <QDesktopServices>

ZTorrent::ZTorrent(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ZTorrent),
    m_engine(new SearchEngine(this)),
    m_transmission(new Transmission(this)),
    m_settings(new Settings(this)),
    m_contextMenu(new QMenu(this))
{
    ui->setupUi(this);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);

    updateActionInfo();

    m_openBrowserMapper = new QSignalMapper(this);
    m_copyLinkMapper = new QSignalMapper(this);
    m_addToTransmissionMapper = new QSignalMapper(this);

    // search finished
    connect(m_engine, SIGNAL(finished(QList<Torrent>)), this, SLOT(showResults(QList<Torrent>)));

    // search error
    connect(m_engine, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleError(QNetworkReply::NetworkError)));

    // torrent action triggered
    connect(m_openBrowserMapper, SIGNAL(mapped(QObject*)), this, SLOT(openBrowser(QObject*)));
    connect(m_copyLinkMapper, SIGNAL(mapped(QObject*)), this, SLOT(copyLink(QObject*)));
    connect(m_addToTransmissionMapper, SIGNAL(mapped(QObject*)), this, SLOT(addToTransmission(QObject*)));

    // torrent added to transmission
    connect(m_transmission, SIGNAL(finished(QString,QString)), this, SLOT(torrentAdded(QString,QString)));
}

ZTorrent::~ZTorrent()
{
    delete ui;
    delete m_engine;
    delete m_transmission;
    delete m_settings;
    delete m_contextMenu;
}

void ZTorrent::updateActionInfo()
{
    int action = m_settings->defaultAction();
    if (action == 0) {
        ui->actionLabel->setText("Default action: copy .torrent link to clipboard");
    }
    else {
        TrBackend b = m_settings->defaultBackend();
        QString host = b["host"].toString();
        QString port = b["port"].toString();
        QString link = QString("http://%1:%2/").arg(host).arg(port);
        QString html = QString("<a href='%1'>%2</a>").arg(link).arg(host);
        ui->actionLabel->setText("Default action: add to download queue at "+html);
    }
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
        cols << torrent["title"] << torrent["size"] << torrent["seeds"]+"/"+torrent["peers"] << torrent["last_added"];
        QTreeWidgetItem* i = new QTreeWidgetItem(ui->treeWidget, cols);
        i->setData(0, Qt::UserRole, QVariant::fromValue(torrent));
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
    QMessageBox::StandardButton btn = QMessageBox::critical(this, "Error", message, QMessageBox::Ok | QMessageBox::Retry);
    if (btn == QMessageBox::Retry) {
        on_lineEdit_returnPressed();
    } else {
        ui->statusBar->showMessage("Error");
    }
}

void ZTorrent::torrentAdded(QString result, QString name)
{
    bool retrying = false;
    if (result == "success") {
        QMessageBox::information(this, "Transmission", name+" added for download");
    } else {
        QMessageBox::StandardButton btn = QMessageBox::critical(this, "Transmission", "Error: "+result, QMessageBox::Ok | QMessageBox::Retry);
        if (btn == QMessageBox::Retry) {
            retrying = true;
            addToTransmission(m_lastAction.first, m_lastAction.second);
        }
    }
    if (!retrying) {
        ui->treeWidget->setEnabled(true);
        ui->statusBar->showMessage("Double click on an item to execute the default action");
    }
}

void ZTorrent::openBrowser(QObject *obj)
{
    QAction *action = qobject_cast<QAction*>(obj);
    QUrl link(action->data().value<Torrent>()["torrentzUrl"]);
    QDesktopServices::openUrl(link);
}

void ZTorrent::copyLink(QObject *obj)
{
    QAction *action = qobject_cast<QAction*>(obj);
    copyToClipboard(action->data().value<Torrent>());
}

void ZTorrent::addToTransmission(QObject *obj)
{
    QAction *action = qobject_cast<QAction*>(obj);
    QList<QVariant> stuff = action->data().toList();
    addToTransmission(stuff[0].value<Torrent>(), stuff[1].value<TrBackend>());
}

void ZTorrent::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    Torrent t = item->data(0, Qt::UserRole).value<Torrent>();
    int act = m_settings->defaultAction();

    if (act == 0) {
        copyToClipboard(t);
    } else if (act == 1) {
        addToTransmission(t, m_settings->defaultBackend());
    }
}

void ZTorrent::copyToClipboard(Torrent t)
{
    QApplication::clipboard()->setText(t["torcacheUrl"]);
    ui->statusBar->showMessage("Link copied to clipboad", 5000);
}

void ZTorrent::addToTransmission(Torrent t, TrBackend backend)
{
    ui->treeWidget->setEnabled(false);
    ui->statusBar->showMessage("Adding "+t["title"]+" for download at "+backend["host"].toString()+"...");
    m_lastAction.first = t; m_lastAction.second = backend;
    m_transmission->addTorrent(t["torcacheUrl"], backend);
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

QAction* ZTorrent::addContextMenuAction(QMenu *menu, const QString &actionText, QVariant &actionData, QSignalMapper *mapper)
{
    QAction *action = menu->addAction(actionText, mapper, SLOT(map()));
    action->setData(actionData);
    mapper->setMapping(action, action);
    return action;
}

void ZTorrent::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeWidget->itemAt(pos);
    QVariant torrentData = item->data(0, Qt::UserRole);

    m_contextMenu->clear();

    addContextMenuAction(m_contextMenu, "Open with browser...", torrentData, m_openBrowserMapper);
    m_contextMenu->addSeparator();
    addContextMenuAction(m_contextMenu, "Copy .torrent link", torrentData, m_copyLinkMapper);

    QList<TrBackend> trBackends = m_settings->backends();
    QMenu *trMenu;
    if (trBackends.size() > 0) {
        trMenu = m_contextMenu->addMenu("Add to transmission");
        foreach (TrBackend b, m_settings->backends()) {
            QString label = b["host"].toString();
            if (b["is_default"].toBool())
                label += " (Default)";

            QList<QVariant> data;
            data << torrentData << QVariant::fromValue(b);
            QVariant trData(data);

            addContextMenuAction(trMenu, label, trData, m_addToTransmissionMapper);
        }
    }

    m_contextMenu->exec(ui->treeWidget->mapToGlobal(pos));
}

void ZTorrent::on_settingsBtn_clicked()
{
    m_settings->exec();
    updateActionInfo();
}

void ZTorrent::on_actionLabel_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}
