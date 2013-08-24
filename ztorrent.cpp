#include "ztorrent.h"
#include "ui_ztorrent.h"
#include "searchengine.h"

#include <QClipboard>

ZTorrent::ZTorrent(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ZTorrent),
    m_engine(new SearchEngine(this))
{
    ui->setupUi(this);
    ui->treeWidget->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setResizeMode(0, QHeaderView::Stretch);

    connect(m_engine, SIGNAL(finished(QList<Torrent>)), this, SLOT(showResults(QList<Torrent>)));
}

ZTorrent::~ZTorrent()
{
    delete ui;
    delete m_engine;
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
    ui->statusBar->showMessage("Search complete. Double click on an item to copy the link to clipboard.");
}

void ZTorrent::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    QString torcache = item->data(0, Qt::UserRole).toString();
    QApplication::clipboard()->setText(torcache);
    ui->statusBar->showMessage("Link copied to clipboad", 5000);
}

void ZTorrent::on_pushButton_clicked()
{
    on_lineEdit_returnPressed();
}

void ZTorrent::on_buttonBox_rejected()
{
    close();
}

void ZTorrent::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button) == QDialogButtonBox::Reset) {
        ui->lineEdit->clear();
        ui->treeWidget->clear();
        ui->statusBar->clearMessage();
        ui->lineEdit->setFocus();
    }
}
