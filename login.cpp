#include "login.h"
#include "ui_login.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include "manager.h"
#include "createuser.h"

login::login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
    ui->lineEdit_user->setPlaceholderText("Username");
    ui->lineEdit_passwd->setPlaceholderText("Password");
}

QString UserKey = "";

login::~login()
{
    delete ui;
}

void login::on_pushButton_login_clicked()
{
    QString username, password;
    QString hashUsername, hashPassword;

    username = ui->lineEdit_user->text();
    password = ui->lineEdit_passwd->text();

    connOpen();
    if(!isconnOpened())
    {
        qDebug() << "Failed to open the database";
        return;
    }

    hashUsername = QString(QCryptographicHash::hash((username.toUtf8()), QCryptographicHash::Md5).toHex());
    hashPassword = QString(QCryptographicHash::hash((password.toUtf8()), QCryptographicHash::Md5).toHex());

    QSqlQuery* qry = new QSqlQuery(dblogin);
    qry->prepare("select * from logindata where userlogin='"+hashUsername+"'");
    qry->exec();
    if(qry->next())
    {
        if(qry->value(2).toString() == hashPassword)
        {
            UserKey = qry->value(3).toString();
            this->hide();
            manager dialog;
            dialog.setModal(true);
            dialog.exec();
        }
        else
        {
            QMessageBox::critical(this, "Error", "Username or password is not correct !");
        }
    }
    else
    {
        if(qry->lastError().text() == "")
            QMessageBox::critical(this, tr("Error"), tr("Given username is not valid!"));
        else
            QMessageBox::critical(this, tr("error::"), qry->lastError().text());
    }
}

void login::on_pushButton_create_clicked()
{
    createuser dialog;
    dialog.setModal(true);
    dialog.exec();
}

