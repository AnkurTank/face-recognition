#include "recognize.h"
#include "ui_recognize.h"

Recognize::Recognize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Recognize)
{
    ui->setupUi(this);
}

Recognize::~Recognize()
{
    delete ui;
}

void Recognize ::closeEvent(QCloseEvent *){

    emit recognize_closed();
}

void Recognize::on_btnBack_clicked()
{
    close();
}
