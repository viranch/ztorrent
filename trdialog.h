#ifndef TRDIALOG_H
#define TRDIALOG_H

#include <QDialog>
#include "trbackend.h"

namespace Ui {
class TrDialog;
}

class TrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TrDialog(QWidget *parent = 0);
    ~TrDialog();

    TrBackend getBackend();
    void loadBackend(TrBackend backend);
    void reset();

private:
    Ui::TrDialog *ui;
};

#endif // TRDIALOG_H
