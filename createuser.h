#ifndef CREATEUSER_H
#define CREATEUSER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QDebug>

namespace Ui {
class createuser;
}

class createuser : public QDialog
{
    Q_OBJECT

    QSqlDatabase dblogin;
    void connClose()                                            //veritabani ile baglanti bitirilmek istenirse...
    {
        dblogin.close();
        dblogin.removeDatabase(QSqlDatabase::defaultConnection);
    }
    bool connOpen()                                             //veritabani ile baglanti olusturmak istenirse...
    {
        dblogin=QSqlDatabase::addDatabase("QSQLITE");
        dblogin.setDatabaseName("C:/Qt/zDB/login.db");           //veritabani konumu

        if(!dblogin.open())
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
    bool isconnOpened()
    {
        return dblogin.isOpen();
    }

public:
    explicit createuser(QWidget *parent = nullptr);
    ~createuser();

private slots:
    void on_pushButton_create_clicked();

private:
    Ui::createuser *ui;
};



#endif // CREATEUSER_H
