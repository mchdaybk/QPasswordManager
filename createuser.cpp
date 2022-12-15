#include "createuser.h"
#include "ui_createuser.h"
#include "manager.h"

createuser::createuser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createuser)
{
    ui->setupUi(this);
    ui->lineEdit_user->setPlaceholderText("Username");
    ui->lineEdit_passwd->setPlaceholderText("Password");
    setWindowIcon(QIcon("C:/Users/msaybek/Desktop/repository/QPasswordManager/key.png"));
    setWindowTitle("Create User Account");
}

createuser::~createuser()
{
    delete ui;
}

void createuser::on_pushButton_create_clicked()
{
    {
        EncryptDecrypt instance;
        QString username=NULL, password=NULL, key=NULL;
        QString hashUsername=NULL, hashPassword=NULL, crypt_key=NULL;
        int last_id=0;
        username = ui->lineEdit_user->text();
        password = ui->lineEdit_passwd->text();
        connOpen();
        if(!isconnOpened())
        {
            qDebug() << "Failed to open the database";
            return;
        }
        do
        {
            key       = instance.getRandomKey();
            crypt_key = instance.encrypt_it(key, instance.getMasterKey());
        }while(crypt_key.contains("00"));                                                   //because if crypted_key has 00 in it, problem occurs when decrypting that key.

        hashUsername = QString(QCryptographicHash::hash((username.toUtf8()), QCryptographicHash::Md5).toHex());
        hashPassword = QString(QCryptographicHash::hash((password.toUtf8()), QCryptographicHash::Md5).toHex());

        QSqlQuery* qry = new QSqlQuery(dblogin);
        qry->prepare("select * from logindata order by id desc limit 1");
        qry->exec();
        if(qry->next())
            last_id = qry->value(0).toInt();

        for(int i=1; i<=last_id; i++)
        {
            qry->prepare("select userlogin from logindata where id = :id");
            qry->bindValue(":id", i);
            qry->exec();
            qry->next();
            if(qry->value(0)==hashUsername)         //controlling username, if exists throw error
            {
                QMessageBox::warning(this, tr("Error"), tr("Given username is used !"));
                return;
            }
        }

        qry->prepare("insert into logindata (userlogin, passwdlogin, key) values('"+hashUsername+"', '"+hashPassword+"', '"+crypt_key+"')");
        if(qry->exec())
        {
            QMessageBox::information(this, tr("Save"), tr("Saved !"));
            QDialog::close();
        }                                                                                                 //veritabani baglantisini kapat..
        else
        {
            QMessageBox::information(this, tr("error::"), qry->lastError().text());                       //bir error gerceklesirse, oldugu gibi goster (exception..)
        }
    }
    connClose();
    //delete qry;
}
