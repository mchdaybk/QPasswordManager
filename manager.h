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


class EncryptDecrypt
{
public:
    unsigned char* key = (unsigned char*)"0123456789abcdef";      //disaridan olusturup veriyoruz..sifreleme icin anahtar
    int cipher_len = 0;
    unsigned char cipher[64];

    QString encrypt_it(QString password)
    {
        QString encrypted_text;
        unsigned char* text = (unsigned char*)StringToChar(password);
        int text_len = strlen((const char*)text);
        memset(&cipher[0], 0, sizeof(cipher));  //cipher sıfırlanıyor ki, temiz bir sekilde sifreleyelim..
        cipher_len = encrypt(text, text_len, key, cipher);  //CİPHER 64'LUK BİR CHAR ARRAY
        free(text);
        encrypted_text = getStringFromUnsignedChar(cipher, cipher_len);
        return encrypted_text;  //SIFRENIN HEX'LI HALI
    }

    /*
encrypt_it ile birlikte girilen parola 64'luk bir char array oluyor kutuphane ile,
biz ise onu hex'e donusturup db'e aktariyoruz..db'de o sekilde bekliyor..sonra ise
db'den hex'li hali aliniyor 64'luk char array haline tekrar getiriliyor cunku
kutuphane fonk'u ancak bu sekilde duzgun calisabiliyor, daha sonra bu 64'luk char
array ilgili fonk'a gonderilip asil girilen parola qstring ile tekrar normal hale
getirilip ilgili yerde ekrana bastiriliyor..
SON DURUMDA BU CLASS FONK'LARI DUZGUN CALISMAKTADIR..
    */

    QString decrypt_it(QString crypt_password, int cipher_len)
    {
        QString decrypted_string;
        unsigned char decrypted[64];    //yeni 64'luk char array, duz parola..
        unsigned char cipher[64];
        for(int i=0;i<cipher_len;++i)   //normalde 32
        {
             QString hexString = crypt_password.mid(i*2,2);
             bool ok = false;
             cipher[i] = (unsigned char)hexString.toUShort(&ok,16);  //if not ok, handle error
        }
        //cipher 64'luk bir char array, password'un donusturulmus hali..
        int dec_len = decrypt(cipher, cipher_len, key, decrypted);
        for(int i=0; i<dec_len; i++)
        {
            decrypted_string += decrypted[i];       //EKK
        }
        return decrypted_string;    //cozulmus string bunun icinde...
    }
};

#endif // MANAGER_H
