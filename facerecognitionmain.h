#ifndef FACERECOGNITIONMAIN_H
#define FACERECOGNITIONMAIN_H

#include <QMainWindow>
#include "entername.h"
#include "recognize.h"

namespace Ui {
class FaceRecognitionMain;
}

class FaceRecognitionMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit FaceRecognitionMain(QWidget *parent = 0);
    ~FaceRecognitionMain();

private slots:
    void on_pbtAddperson_clicked();
    void clear_entername();
    void clear_recognize();

    void on_pbtRecognizeperson_clicked();

private:
    Ui::FaceRecognitionMain *ui;
    /*Enter name dialog pointer*/
    EnterName * my_enterName;

    /*Recognize dialog pointer*/
    Recognize * my_recognize;

};

#endif // FACERECOGNITIONMAIN_H
