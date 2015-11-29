#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include <QDialog>
#include "getcamframe.h"

namespace Ui {
class Recognize;
}

class Recognize : public QDialog
{
    Q_OBJECT

public:
    explicit Recognize(QWidget *parent = 0);
    ~Recognize();

protected:
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *ev);

private:
    Ui::Recognize *ui;
    getcamframe * m_getFramForRec;
    bool m_capture_done;
    QThread *m_pthread;

    CascadeClassifier faceCascade;
    CascadeClassifier eyeCascade1;
    CascadeClassifier eyeCascade2;

signals:
    void recognize_closed();
    void window_loaded();
private slots:
    void on_btnBack_clicked();
    void Recognize_face();
};

#endif // RECOGNIZE_H
