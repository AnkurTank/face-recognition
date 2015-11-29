#include "entername.h"
#include "ui_entername.h"
#include "QDebug"
#include "common_cv.h"


EnterName::EnterName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnterName)
{
    ui->setupUi(this);
    my_captureface = NULL;
}

EnterName::~EnterName()
{
    delete ui;
}

void EnterName::clear_captureface()
{

    if (this->my_captureface != NULL) {
        delete this->my_captureface;
        this->my_captureface = NULL;
    }
    //this->show();
    //Go Back to FaceReconitionMain
    emit entername_closed();
}

void EnterName::on_btnAdd_clicked()
{
       g_personname = ui->lneEntername->text();
       qDebug() << "Entered name is :" << g_personname;

       m_enteredName = ui->lneEntername->text().toStdString();
       ui->lneEntername->clear();

       if (this->my_captureface == NULL) {
           this->my_captureface = new CaptureFace(this);
       }
       this->hide();
       connect(my_captureface,SIGNAL(captureface_closed()),this,SLOT(clear_captureface()));
       this->my_captureface->show();
}

void EnterName::closeEvent(QCloseEvent *)
{
    emit entername_closed();
}

void EnterName::on_btnBack_clicked()
{
    this->ui->lneEntername->clear();
    this->close();
}
