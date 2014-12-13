#include "captureface.h"
#include "ui_captureface.h"

CaptureFace::CaptureFace(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CaptureFace)
{
    ui->setupUi(this);
}

CaptureFace::~CaptureFace()
{
    delete ui;
}

void CaptureFace::closeEvent(QCloseEvent * )
{
    emit captureface_closed();
}

void CaptureFace::on_pbtBack_clicked()
{
    close();
}
