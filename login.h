#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>
#include "manager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class login; }
QT_END_NAMESPACE

class login : public QMainWindow
{
    Q_OBJECT

    /*
    public:
        QSqlDatabase mydb;
        void connClose()
        {
            mydb.close();
            mydb.removeDatabase(QSqlDatabase::defaultConnection);
        }
        bool connOpen()
        {
            mydb=QSqlDatabase::addDatabase("QSQLITE");           //burada QSqlDatabase mydb ile basliyordu
            mydb.setDatabaseName("C:/Qt/zDB/passwdmanager.db");  //satir, header'a eklendi burdan silindi

            if(!mydb.open())
            {
                qDebug() << "Failed to open the database";
                return false;
            }
            else
            {
                qDebug() << "Connected...";
                return true;
            }
        }
    */

            //eger login bilgilerininde veritabanindan cekilmesi istenirse
            //buradaki ve login.cpp'deki yildizlari kaldirip, veritabani
            //konumu ayarlandiktan sonra calistirilmasi yeterli olur..
            //ancak veritabani baglantisi diger kisim (manager) lazim
            //oldugu icin, o tarafin header'inda kullaniliyor..

public:
    login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_pushButton_login_clicked();

private:
    Ui::login *ui;
};
#endif // LOGIN_H
