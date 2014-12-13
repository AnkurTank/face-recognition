#include "entername.h"
#include "ui_entername.h"
#include "QDebug"


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
    this->show();
}

void EnterName::on_btnAdd_clicked()
{
       QString strPersonName;
       strPersonName = ui->lneEntername->text();
       qDebug() << "Entered name is :" << strPersonName;

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
