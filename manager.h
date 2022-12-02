#ifndef MANAGER_H
#define MANAGER_H

#include "openssl/evp.h"
#include "openssl/conf.h"
#include "passwdsource.h"
#include <QDialog>
#include <QString>
#include <QCheckBox>
#include <QDebug>
#include <QComboBox>
#include <QMessageBox>
#include <QtSql>
#include <QFileInfo>
#include <QTableWidget>
#include <QTableWidgetItem>


namespace Ui {
class manager;
}

class manager : public QDialog
{
    Q_OBJECT

public:
    QSqlDatabase mydb;
    void connClose()                                            //veritabani ile baglanti bitirilmek istenirse...
    {
        {
        mydb.close();
        }
        mydb.removeDatabase(QSqlDatabase::defaultConnection);
    }
    bool connOpen()                                             //veritabani ile baglanti olusturmak istenirse...
    {
        mydb=QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("C:/Qt/zDB/manager.db");           //veritabani konumu

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
    bool isconnOpened()
    {
        return mydb.isOpen();
    }

public:
    explicit manager(QWidget *parent = nullptr);
    ~manager();

private slots:
    void on_pushButton_generate_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_ctrl_clicked();

    void on_pushButton_ctrl_save_clicked();

    void on_pushButton_show_clicked();

    void on_pushButton_delete_clicked();

    void cellClicked();

private:
    Ui::manager *ui;
};

#endif // MANAGER_H
