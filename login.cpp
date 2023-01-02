#include "login.h"
#include "ui_login.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFile>
#include <QCoreApplication>
#include "manager.h"
#include "createuser.h"

login::login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
    ui->lineEdit_user->setPlaceholderText("Username");
    ui->lineEdit_passwd->setPlaceholderText("Password");
    setWindowIcon(QIcon("C:/Users/msaybek/Desktop/repository/QPasswordManager/key.png"));
    setWindowTitle("Q Password Manager");


    if(!QFileInfo::exists("C:/Users/msaybek/Desktop/ZZZZ/database.db"))
    {
        QSqlDatabase dbcreate = QSqlDatabase::addDatabase("QSQLITE");
        dbcreate.setDatabaseName("C:/Users/msaybek/Desktop/ZZZZ/database.db");
        if(dbcreate.open())
        {
            QSqlQuery qry;
            qry.prepare("CREATE TABLE IF NOT EXISTS data (id INTEGER PRIMARY KEY AUTOINCREMENT, username varchar(64), password varchar(64), source varchar(64), key varchar(64))");
            if(!qry.exec())
            {
                QMessageBox::warning(this, tr("error::"), qry.lastError().text());
            }
        }
        else
        {
            QMessageBox::warning(this, "Error", "Error here");
        }
    }
    else
    {
        qDebug() << "Database already created..";
    }
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
            UserKey = qry->value(3).toString();                             //getting users key to use it in app and show only users values..
            this->hide();
            connClose();
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

