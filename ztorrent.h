#ifndef ZTORRENT_H
#define ZTORRENT_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QAbstractButton>

#include "torrent.h"

namespace Ui {
class ZTorrent;
}

class SearchEngine;

class ZTorrent : public QMainWindow
{
    Q_OBJECT

public:
    explicit ZTorrent(QWidget *parent = 0);
    ~ZTorrent();

private slots:
    void on_lineEdit_returnPressed();
    void showResults(QList<Torrent> torrents);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_pushButton_clicked();
    void on_buttonBox_rejected();
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::ZTorrent *ui;
    SearchEngine *m_engine;
};

#endif // ZTORRENT_H
