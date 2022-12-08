#include "createuser.h"
#include "ui_createuser.h"
#include "manager.h"

createuser::createuser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createuser)
{
    ui->setupUi(this);
}

createuser::~createuser()
{
    delete ui;
}

void createuser::on_pushButton_create_clicked()
{
    EncryptDecrypt instance;
    QString username, password, key;
    QString hashUsername, hashPassword, crypt_key;
    username = ui->lineEdit_user->text();
    password = ui->lineEdit_passwd->text();
    connOpen();
    if(!isconnOpened())
    {
        qDebug() << "Failed to open the database";
        return;
    }
    key          = instance.GetKey();
    crypt_key    = instance.encrypt_it(key, instance.getMainKey());
    hashUsername = QString(QCryptographicHash::hash((username.toUtf8()),QCryptographicHash::Md5).toHex());
    hashPassword = QString(QCryptographicHash::hash((password.toUtf8()),QCryptographicHash::Md5).toHex());

    QSqlQuery* qry = new QSqlQuery(dblogin);
    qry->prepare("insert into logindata (userlogin, passwdlogin, key) values('"+hashUsername+"', '"+hashPassword+"', '"+crypt_key+"')");
    if(qry->exec())
    {
        QMessageBox::information(this, tr("Save"), tr("Saved !"));
    }                                                                                                //veritabani baglantisini kapat..
    else
    {
        QMessageBox::information(this, tr("error::"), qry->lastError().text());                       //bir error gerceklesirse, oldugu gibi goster (exception..)
    }
    {
        connClose();
    }
    delete qry;
}
