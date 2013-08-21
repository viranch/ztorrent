#include "ztorrent.h"
#include "ui_ztorrent.h"
#include "searchengine.h"
#include "feeditem.h"

#include <QClipboard>

ZTorrent::ZTorrent(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ZTorrent),
    m_engine(new SearchEngine(this))
{
    ui->setupUi(this);

    connect(m_engine, SIGNAL(finished(QList<FeedItem*>)), this, SLOT(showResults(QList<FeedItem*>)));
}

ZTorrent::~ZTorrent()
{
    delete ui;
}

void ZTorrent::on_lineEdit_returnPressed()
{
    ui->statusBar->showMessage("Searching...");
    m_engine->search(ui->lineEdit->text());
}

void ZTorrent::showResults(QList<FeedItem*> feeds)
{
    ui->listWidget->clear();
    foreach(FeedItem* feed, feeds) {
        QListWidgetItem* i = new QListWidgetItem(feed->property("title"), ui->listWidget);
        i->setWhatsThis(feed->property("link"));
    }
    ui->statusBar->showMessage("Search complete. Double click on an item to copy the link to clipboard.");
}

void ZTorrent::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString filename = item->text().replace(' ', '.').toLower();
    QString hash = item->whatsThis().split('/').last().toUpper();
    QString torcache = "http://torcache.net/torrent/"+hash+".torrent?title="+filename;
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
        ui->listWidget->clear();
        ui->statusBar->clearMessage();
        ui->lineEdit->setFocus();
    }
}
