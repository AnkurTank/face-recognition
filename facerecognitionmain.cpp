#include "facerecognitionmain.h"
#include "ui_facerecognitionmain.h"


FaceRecognitionMain::FaceRecognitionMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FaceRecognitionMain)
{
    ui->setupUi(this);
    this->my_enterName = NULL;
    this->my_recognize = NULL;
}

FaceRecognitionMain::~FaceRecognitionMain()
{
    delete ui;
}
void FaceRecognitionMain::clear_entername()
{
    if (this->my_enterName != NULL) {
        delete this->my_enterName;
        this->my_enterName = NULL;
    }
    this->show();
}

void FaceRecognitionMain::on_pbtAddperson_clicked()
{
    if (this->my_enterName == NULL) {
        this->my_enterName = new EnterName();
    }
    this->hide();
    connect(my_enterName,SIGNAL(entername_closed()),this,SLOT(clear_entername()));
    this->my_enterName->show();
}
void FaceRecognitionMain::clear_recognize()
{
    if (this->my_recognize != NULL) {
        delete this->my_recognize;
        this->my_recognize = NULL;
    }
    this->show();
}
void FaceRecognitionMain::on_pbtRecognizeperson_clicked()
{
    if (this->my_recognize == NULL) {
        this->my_recognize = new Recognize();
    }
    this->hide();
    connect(my_recognize,SIGNAL(recognize_closed()),this,SLOT(clear_recognize()));
    this->my_recognize->show();

}
