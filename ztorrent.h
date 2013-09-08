#ifndef ZTORRENT_H
#define ZTORRENT_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QAbstractButton>
#include <QNetworkReply>

#include "torrent.h"
#include "trbackend.h"

namespace Ui {
class ZTorrent;
}

class SearchEngine;
class Transmission;
class Settings;

class ZTorrent : public QMainWindow
{
    Q_OBJECT

public:
    explicit ZTorrent(QWidget *parent = 0);
    ~ZTorrent();

private slots:
    void on_lineEdit_returnPressed();
    void showResults(QList<Torrent> torrents);
    void torrentAdded(QString result, QString name);
    void handleError(QNetworkReply::NetworkError e);
    void copyToClipboard(QString hash);
    void addToTransmission(QString hash, TrBackend backend);
    void menuAction(QAction *action);

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_goBtn_clicked();
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_settingsBtn_clicked();

private:
    Ui::ZTorrent *ui;
    SearchEngine *m_engine;
    Transmission *m_transmission;
    Settings *m_settings;
    QMenu *m_contextMenu;
};

#endif // ZTORRENT_H
