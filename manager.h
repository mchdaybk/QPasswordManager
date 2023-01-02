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
#include <QMouseEvent>

int encrypt(unsigned char*, int, unsigned char*, unsigned char*);
int decrypt(unsigned char*, int, unsigned char*, unsigned char*);
unsigned char* StringToChar(QString);
QString getStringFromUnsignedChar(unsigned char[64],int);

namespace Ui {
class manager;
}

class manager : public QDialog
{
    Q_OBJECT

public:
    QSqlDatabase mydb;
    void connClose()                                            //close connection to the database
    {
        QString connection;
        connection = mydb.connectionName();
        mydb = QSqlDatabase();
        QSqlDatabase::removeDatabase(connection);
        qDebug() << "Disconnected...manager";
        return;
    }
    bool connOpen()                                             //open connection to the database
    {
        mydb=QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName(db_location);           //db location

        if(!mydb.open())
        {
            qDebug() << "Failed to open the database";
            return false;
        }
        else
        {
            qDebug() << "Connected...manager";
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

    void on_pushButton_deleteUser_clicked();

    void on_pushButton_deleteUser_2_clicked();

    void cellClicked();

private:
    Ui::manager *ui;

};


class EncryptDecrypt
{
public:
    //unsigned char* key = (unsigned char*)"0123456789abcdef";
    int cipher_len = 0;
    unsigned char cipher[64];

    QString encrypt_it(QString password, unsigned char* key)
    {
        QString encrypted_text;
        unsigned char* text = (unsigned char*)StringToChar(password);
        int text_len = strlen((const char*)text);
        memset(&cipher[0], 0, sizeof(cipher));              //to clean encryption process, cipher array cleaned
        cipher_len = encrypt(text, text_len, key, cipher);  //cipher is the 64 element char array and it should be according to the openssl's aes128 function
        free(text);
        encrypted_text = getStringFromUnsignedChar(cipher, cipher_len);
        return encrypted_text;                              //password in hex
    }

    /*
     * due to the "encrypt_it" function, the password converting to the unsigned
     * char array[64] by the library itself. In this program, this array converting
     * to the hex value then store in the database. Through this process, given
     * password encrypted. For decryption, stored hex value fetching from database
     * then converting to the QString type then converting char array then decrypting
     * process then the password obtained as plain text (no encrypted value) in QString.
     */

    QString decrypt_it(QString crypt_password, int cipher_len, unsigned char* key)
    {
        QString decrypted_string;
        unsigned char decrypted[64];    //new password actually, 64 element char array
        unsigned char cipher[64];
        for(int i=0;i<cipher_len;++i)   //its 32 in initial state(openssl)
        {
             QString hexString = crypt_password.mid(i*2,2);
             bool ok = false;
             cipher[i] = (unsigned char)hexString.toUShort(&ok,16);  //if not ok, handle error
        }
        //cipher is 64 element char array, it stores converted password
        int dec_len = decrypt(cipher, cipher_len, key, decrypted);
        for(int i=0; i<dec_len; i++)
        {
            decrypted_string += QChar(decrypted[i]);
        }
        return decrypted_string;
    }

    QString getRandomKey()
    {
        const int randomStringLength = 16;      //key length
        QString randomKey;
        for(int i=0; i<randomStringLength; ++i)
        {
            int index = QRandomGenerator::global()->bounded(0, Characters.length());
            QChar nextChar = Characters.at(index);
            randomKey.append(nextChar);
        }
        return randomKey;      //returns unsigned char* of random generated key.
    }

    unsigned char* getMasterKey()
    {
        return masterKey;
    }

private:
    unsigned char* masterKey = (unsigned char*)"abcdef0123456789";          //master key of the program...this key encrypt to generated keys
    const QString Characters = "a0bc1de2fg3hi4jk5lm6no7pr8st9vyqzw";        //character set to the generate keys
};




#endif // MANAGER_H
