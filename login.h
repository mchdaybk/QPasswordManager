#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>
#include "manager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class login; }
QT_END_NAMESPACE

extern QString UserKey;

class login : public QMainWindow
{
    Q_OBJECT


public:
    QSqlDatabase dblogin;
    void connClose()
    {
        QString connection;
        connection = dblogin.connectionName();
        dblogin = QSqlDatabase();
        QSqlDatabase::removeDatabase(connection);
        qDebug() << "Disconnected...login";
        return;
    }
    bool connOpen()
    {
        dblogin=QSqlDatabase::addDatabase("QSQLITE");
        dblogin.setDatabaseName("C:/Qt/zDB/manager.db");

        if(!dblogin.open())
        {
            qDebug() << "Failed to open the database";
            return false;
        }
        else
        {
            qDebug() << "Connected...login";
            return true;
        }
    }
    bool isconnOpened()
    {
        return dblogin.isOpen();
    }

public:
    login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_create_clicked();

private:
    Ui::login *ui;
};
#endif // LOGIN_H
