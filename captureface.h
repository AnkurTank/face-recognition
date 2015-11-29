#ifndef CAPTUREFACE_H
#define CAPTUREFACE_H

#include <QDialog>
#include "getcamframe.h"

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

    //void on_pushButton_clicked();

    void on_pbtDone_clicked();

    void on_btnStartCapture_clicked();

    void captureface();

private:
    Ui::CaptureFace *ui;
    bool m_capture_done;
    QThread *m_pthread;
    getcamframe * m_getcamframe;

    CascadeClassifier faceCascade;
    CascadeClassifier eyeCascade1;
    CascadeClassifier eyeCascade2;
    int MaximumFace2Becaptured;

signals:
    void captureface_closed();
};

#endif // CAPTUREFACE_H
