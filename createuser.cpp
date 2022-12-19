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
        if(username!="" || password!="")
        {
            do
            {
                key       = instance.getRandomKey();
                crypt_key = instance.encrypt_it(key, instance.getMasterKey());
            }while(crypt_key.contains("00"));                                                   //because if crypted_key has 00 in it, problem occurs when decrypting that key.

            hashUsername = QString(QCryptographicHash::hash((username.toUtf8()), QCryptographicHash::Md5).toHex());
            hashPassword = QString(QCryptographicHash::hash((password.toUtf8()), QCryptographicHash::Md5).toHex());
            connOpen();
            if(!isconnOpened())
            {
                qDebug() << "Failed to open the database";
                return;
            }
            QSqlQuery* qry = new QSqlQuery(dblogin);
            qry->prepare("select * from logindata order by id desc limit 1");                   //if there is no record in the db, program do not enter inside this if..
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
                    connClose();
                    return;
                }
            }

            qry->prepare("insert into logindata (userlogin, passwdlogin, key) values('"+hashUsername+"', '"+hashPassword+"', '"+crypt_key+"')");
            if(qry->exec())
            {
                QMessageBox::information(this, tr("Save"), tr("Saved !"));
                QDialog::close();
            }
            else
            {
                QMessageBox::information(this, tr("error::"), qry->lastError().text());        //if an error occures, throw this error (exception..)
            }
        }
        else
        {
            QMessageBox::information(this, tr("Error"), tr("Please enter valid username and password !"));
        }
    }
    connClose();
}
