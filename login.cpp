#include "login.h"
#include "ui_login.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include "manager.h"


login::login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}


void login::on_pushButton_login_clicked()
{
    QString username,password;
    username = ui->lineEdit_user->text();
    password = ui->lineEdit_passwd->text();

    if((username == "1" && password == "1"))        //admin ve 12345...istegine gore ayarla..
    {
        manager secdialog;
        secdialog.connClose();          //baglanti var ise, veritabaniyla baglanti kapatilip, tekrar aciliyor..
        this->hide();                   //giris bilgileri dogru ise
        secdialog.setModal(true);       //asil uygulama ekrani
        secdialog.exec();               //aciliyor..
    }
    else
    {
        QMessageBox::critical(this, "Error", "Username or password is not correct !");
    }

    /*
    if(!connOpen())
    {
        qDebug() << "Failed to open the database";
        return;
    }
    connOpen();
    QSqlQuery qry;                                                          //nesne olusturuldu, database ile o an da
    qry.prepare("select * from employeeinfo where username='"+username+"' and password='"+password+"'");
                                                                            //ancak bir tane connection olusturulabiliyor
    if(qry.exec())
    {
        int count = 0;
        while(qry.next())
        {
            count++;
        }
        if(count==1)                                                        //parola dogru girildikten sonra, ilk ekran kapanicak
        {                                                                   //ikinci ekran acilacak..(dialog screen)
            ui->label_bos->setText("Username and password is correct");     //veriler dogru oldugu
            connClose();                                                    //icin db ile baglantiyi kapatmak gerek
            this->hide();
            manager secdialog;
            secdialog.setModal(true);                                       //bu sekilde manager.ui acilacak
            secdialog.exec();
        }
        if(count>1)
            ui->label_bos->setText("Duplicate username and password !");
        if(count<1)
            ui->label_bos->setText("Username and password is not correct !");
    }
    */
}

