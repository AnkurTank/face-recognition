#ifndef CAPTUREFACE_H
#define CAPTUREFACE_H

#include <QDialog>

namespace Ui {
class CaptureFace;
}

class CaptureFace : public QDialog
{
    Q_OBJECT

public:
    explicit CaptureFace(QWidget *parent = 0);
    ~CaptureFace();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void on_pbtBack_clicked();

private:
    Ui::CaptureFace *ui;
signals:
    void captureface_closed();
};

#endif // CAPTUREFACE_H
