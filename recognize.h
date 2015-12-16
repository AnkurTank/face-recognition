#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include <QDialog>
#include "getcamframe.h"
#include "common_cv.h"

namespace Ui {
class Recognize;
}

class Recognize : public QDialog
{
    Q_OBJECT

public:
    explicit Recognize(QWidget *parent = 0);
    ~Recognize();
    bool initializeFaceRecognizer();
    bool storeFaceNames(QString Filename);
    bool readBackFaceNames(QString Filename);

protected:
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *ev);

private:
    Ui::Recognize *ui;
    getcamframe * m_getFramForRec;
    bool m_capture_done;
    QThread *m_pthread;
    Ptr<FaceRecognizer> model;

    CascadeClassifier faceCascade;
    CascadeClassifier eyeCascade1;
    CascadeClassifier eyeCascade2;

signals:
    void recognize_closed();
    void window_loaded();
private slots:
    void Recognize_face();
    void on_btnRecBack_clicked();
};

#endif // RECOGNIZE_H
