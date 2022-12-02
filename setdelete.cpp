#include "setdelete.h"
#include "ui_setdelete.h"

setdelete::setdelete(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setdelete)
{
    ui->setupUi(this);
}

setdelete::~setdelete()
{
    delete ui;
}

QString setdelete::getId()
{
    QString id = ui->lineEdit->text();
    return id;
}

void setdelete::on_pushButton_clicked()
{
    //getString();
    hide();
}

