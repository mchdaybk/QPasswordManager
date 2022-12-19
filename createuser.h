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
    void connClose()
    {
        QString connection;
        connection = dblogin.connectionName();
        dblogin = QSqlDatabase();
        QSqlDatabase::removeDatabase(connection);
        qDebug() << "Disconnected...createuser";
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
            qDebug() << "Connected...createuser";
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
