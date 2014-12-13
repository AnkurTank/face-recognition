#ifndef ENTERNAME_H
#define ENTERNAME_H

#include <QDialog>
#include "captureface.h"

namespace Ui {
class EnterName;
}

class EnterName : public QDialog
{
    Q_OBJECT

public:
    explicit EnterName(QWidget *parent = 0);
    ~EnterName();
protected:
    void closeEvent(QCloseEvent *);

private slots:
    void on_btnAdd_clicked();

    void on_btnBack_clicked();
    void clear_captureface();

private:
    Ui::EnterName *ui;
    CaptureFace * my_captureface;
signals:
    void entername_closed();

};

#endif // ENTERNAME_H
