#include "manager.h"
#include "ui_manager.h"
#include <QItemSelectionModel>
#include <QAbstractItemModel>
#include <QDialog>
#include <QClipboard>
#include "login.h"


int deneme();
bool letter_special_number(QString);
bool letter_special(QString);
bool letter_number(QString);
bool special_number(QString);
bool letter(QString);
bool special(QString);
bool number(QString);



manager::manager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::manager)
{
    ui->setupUi(this);
    ui->comboBox->addItem("8", 8);      //the numbers inside the quotations marks are
    ui->comboBox->addItem("9", 9);      //the numbers we want to show and the numbers
    ui->comboBox->addItem("10", 10);    //located right, the value of choice.
    ui->comboBox->addItem("11", 11);
    ui->comboBox->addItem("12", 12);
    ui->comboBox->addItem("13", 13);
    ui->comboBox->addItem("14", 14);
    ui->comboBox->addItem("15", 15);
    ui->comboBox->addItem("16", 16);
    ui->comboBox->addItem("17", 17);
    ui->comboBox->addItem("18", 18);
    ui->comboBox->addItem("19", 19);
    ui->comboBox->addItem("20", 20);

    setWindowIcon(QIcon("C:/Users/msaybek/Desktop/repository/QPasswordManager/key.png"));
    setWindowTitle("Q Password Manager");
    ui->lineEdit->setPlaceholderText("Generate password with above choices");
    ui->lineEdit_ctrl->setPlaceholderText("Enter the password you want to control");
    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &manager::cellClicked);
    connOpen();

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //below signal is for copy the password when right click on it
    connect(ui->tableWidget, &QWidget::customContextMenuRequested,this,[this]()
    {
        QList<QTableWidgetItem *> items=ui->tableWidget->selectedItems();
        if(items.count()>0)
            qDebug()<<items[0]->text();
        QString copypasswd = items[0]->text();
        qApp->clipboard()->setText(copypasswd);
    });
}



//passwords decrypted in below function..

void manager::cellClicked()
{
    {
    QString password=NULL, password_d=NULL, key=NULL, id_str=NULL;
    int id=0, row=0, column=0;
    unsigned char* u_key=NULL;
    EncryptDecrypt instance;
    QTableWidgetItem* item = new QTableWidgetItem();

    QModelIndexList indexes = ui->tableWidget->selectionModel()->selectedIndexes();
    foreach (QModelIndex index, indexes)
    {
        row = index.row();
        column = index.column();
    }
    if(column == 1)
    {
        if(ui->tableWidget->item(row,1)->text() == "********")
        {
//            connOpen();
            QSqlQuery* qry = new QSqlQuery(mydb);
            id_str = ui->tableWidget->item(row,3)->text();                          //get id number of selected cell which was same with the id number in database
            id = id_str.split(" ")[0].toInt();                                      //string to integer conversion..
            qry->prepare("select * from data where id = :id");
            qry->bindValue(":id", id);
            qry->exec();
            if(qry->next())
            {
                password = qry->value(2).toString();                                //get password
                key = qry->value(4).toString();                                     //get users key
                key = instance.decrypt_it(key, key.length()/2, instance.getMasterKey());
                u_key = (unsigned char*)StringToChar(key);
                password_d = instance.decrypt_it(password, password.length()/2, u_key);
                ui->tableWidget->item(row,1)->setText(password_d);                    //there is an item in the tablewidget so we can use "item" method..
                delete qry;                                                           //if there would not be an item in the tablewidget we could not use
//                connClose();                                                        //"setText" method..
            }
            else
            {
                QMessageBox::critical(this, tr("error::"), qry->lastError().text());
            }
        }
        else
        {
            ui->tableWidget->item(row,1)->setText("********");
        }
    }
    delete item;
    free(u_key);
    }
}


class GetRandom
{
public:
    GetRandom(){}

    QString GetRandomStr(int size)
    {
        const int randomStringLength = size;
        QString randomString;
        for(int i=0; i<randomStringLength; ++i)
        {
            //int index = qrand() % possibleCharacters.length();
            int index = QRandomGenerator::global()->bounded(0, possibleCharacters.length());
            QChar nextChar = possibleCharacters.at(index);
            randomString.append(nextChar);
        }
        return randomString;
    }
    void setCharacter_Number()
    {
        possibleCharacters += "0123456789";
    }
    void setCharacter_Special()
    {
        possibleCharacters += ".,!'#+%-_";
    }
    void setCharacter_Big()
    {
        possibleCharacters += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    void setCharacter_Number_Special()
    {
        possibleCharacters += "0123456789.,!'#+%-_";
    }
    void setCharacter_Big_Special()
    {
        possibleCharacters += "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,!'#+%-_";
    }
    void setCharacter_Number_Big()
    {
        possibleCharacters += "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    }
    void setCharacter_Number_Big_Special()
    {
        possibleCharacters += "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!'#+%-_";
    }
private:
    QString possibleCharacters = "abcdefghijklmnopqrstuvwxyz";
};

class ControlPasswd
{
public:
    ControlPasswd(){}

    bool Control_Letter(QString control)
    {
        QString letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        for(int i=0; i<=letters.length(); i++)
        {
            if(control.contains(letters[i]))
            {
                return true;            //if given password has big letters
            }                           //return true..or false
        }
        return false;
    }
    bool Control_Special(QString control)
    {
        QString special = ".,!'#+%-_";
        for(int i=0; i<=special.length(); i++)
        {
            if(control.contains(special[i]))
            {
                return true;            //if given password has special characters
            }                           //return true..or false
        }
        return false;
    }
    bool Control_Number(QString control)
    {
        QString numbers = "0123456789";
        for(int i=0; i<=numbers.length(); i++)
        {
            if(control.contains(numbers[i]))
            {
                return true;            //if given password has special characters
            }                           //return true..or false
        }
        return false;
    }

};

unsigned char* StringToChar(QString string_tobe_char)
{
    QByteArray ba = string_tobe_char.toLocal8Bit();
    unsigned char* converted_char = (unsigned char*)strdup(ba.constData());
    return converted_char;
}

//unsigned char pointer --unsigned char* str-- verildiginde array
//icerisinde -null- elemani varsa, elemani gorene kadar array'i
//aliyor ancak -null- dan sonraki uyeler kayboluyor cunku
//array'i bitmis kabul ediyor bu da verinin yanlis saklanmasina
//sebep oluyor o da crash yapiyor..

QString getStringFromUnsignedChar(unsigned char str[64], int cipher_len){
    QString result = "";
    //int lengthOfString = strlen( reinterpret_cast<const char*>(str) );

    // print string in reverse order
    QString s;
    for( int i = 0; i < cipher_len; i++ ){
        s = QString( "%1" ).arg( str[i], 0, 16 );

        // account for single-digit hex values (always must serialize as two digits)
        if( s.length() == 1 )
            result.append( "0" );

        result.append( s );
    }

    return result;
}
//below functions are openssl's aes128 functions for encryption and decryption
int encrypt(unsigned char* text, int text_len, unsigned char* key, unsigned char* cipher){
    int cipher_len = 0;
    int len = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if(!ctx)
    {
        perror("EVP_CIPHER_CTX_new() failed");
        exit(-1);
    }
    if(!EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
    {
        perror("EVP_EncryptInit_ex() failed");
        exit(-1);
    }
    if(!EVP_EncryptUpdate(ctx, cipher, &len, text, text_len))
    {
        perror("EVP_EncrypUpdate() failed");
        exit(-1);
    }

    cipher_len += len;

    if(!EVP_EncryptFinal_ex(ctx, cipher + len, &len))
    {
        perror("EVP_EncryptFinal_ex() failed");
        exit(-1);
    }

    cipher_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return cipher_len;
}

int decrypt(unsigned char* cipher, int cipher_len, unsigned char* key, unsigned char* text)
{
    int text_len = 0;
    int len = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if(!ctx)
    {
        perror("EVP_CIPHER_CTX_new() failed");
        exit(-1);
    }
    if(!EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
    {
        perror("EVP_DecryptInit_ex() failed");
        exit(-1);
    }
    if(!EVP_DecryptUpdate(ctx, text, &len, cipher, cipher_len))
    {
        perror("EVP_DecryptUpdate() failed");
        exit(-1);
    }

    text_len += len;

    if(!EVP_DecryptFinal_ex(ctx, text + len, &len))
    {
        perror("EVP_DecryptFinal_ex() failed");
        exit(-1);
    }

    text_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return text_len;
}



manager::~manager()
{
    connClose();
    delete ui;
}



void manager::on_pushButton_generate_clicked()
{
    QString newstring;
    GetRandom instance;
    int size = ui->comboBox->currentData().toInt();                                //both are work..this line and below line..
    //int size = ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt();

    if(ui->checkBox_number->isChecked())                                           //converting int and assign a variable
    {
        if(ui->checkBox_letter->isChecked())
        {
            if(ui->checkBox_special->isChecked())
            {
                instance.setCharacter_Number_Big_Special();
                do
                {
                    newstring = instance.GetRandomStr(size);    //number, bigletter, special character case
                }while(!letter_special_number(newstring));
                ui->lineEdit->setText(newstring);
            }
            else
            {
                instance.setCharacter_Number_Big();
                do
                {
                newstring = instance.GetRandomStr(size);
                }while(!letter_number(newstring));
                ui->lineEdit->setText(newstring);               //number and big letter case
            }
        }
        else
        {
            instance.setCharacter_Number();
            do
            {
            newstring = instance.GetRandomStr(size);
            }while(!number(newstring));
            ui->lineEdit->setText(newstring);                   //number case
        }
    }
    else if(ui->checkBox_letter->isChecked())
    {
        if(ui->checkBox_special->isChecked())
        {
            instance.setCharacter_Big_Special();
            do
            {
            newstring = instance.GetRandomStr(size);
            }while(!letter_special(newstring));
            ui->lineEdit->setText(newstring);                   //big letter and special character case
        }
        else
        {
            instance.setCharacter_Big();
            do{
            newstring = instance.GetRandomStr(size);
            }while(!letter(newstring));
            ui->lineEdit->setText(newstring);                   //big letter case
        }
    }
    else if(ui->checkBox_special->isChecked())
    {
        if(ui->checkBox_number->isChecked())
        {
            instance.setCharacter_Number_Special();
            do
            {
            newstring = instance.GetRandomStr(size);
            }while(!special_number(newstring));
            ui->lineEdit->setText(newstring);                   //special character and number case
        }
        else
        {
            instance.setCharacter_Special();
            do{
            newstring = instance.GetRandomStr(size);
            }while(!special(newstring));
            ui->lineEdit->setText(newstring);                   //special character case
        }
    }
    else
    {
        newstring = instance.GetRandomStr(size);
        ui->lineEdit->setText(newstring);                       //only small letter case
    }
}



void manager::on_pushButton_ctrl_clicked()
{
    ControlPasswd nesne;
    QString password_ctrl = ui->lineEdit_ctrl->text();               //get text from line edit
    if(password_ctrl.length() >= 8 && password_ctrl.length() <= 20)  //character boundary
    {
        if(!ui->checkBox_number_ctrl->isChecked() && !ui->checkBox_letter_ctrl->isChecked() && !ui->checkBox_special_ctrl->isChecked())
        {
            QMessageBox::information(this, "Message", "Please give at least one property from the boxes !");
        }
        else if(ui->checkBox_number_ctrl->isChecked() && nesne.Control_Number(password_ctrl))      //control special characters
        {
            if(ui->checkBox_letter_ctrl->isChecked() && nesne.Control_Letter(password_ctrl))       //control numbers
            {
                if(ui->checkBox_special_ctrl->isChecked() && nesne.Control_Special(password_ctrl)) //control big letters
                {
                    QMessageBox::information(this, "Message", "OK! The password has special characters and numbers and big letters !");
                }
                else
                {
                    QMessageBox::information(this, "Message", "OK! The password has both big letters and numbers !");
                }
            }
            else
            {
                QMessageBox::information(this, "Message", "OK! The password has only numbers !");
            }
        }
        else if(ui->checkBox_letter_ctrl->isChecked() && nesne.Control_Letter(password_ctrl))
        {
            if(ui->checkBox_special_ctrl->isChecked() && nesne.Control_Special(password_ctrl))
            {
                QMessageBox::information(this, "Message", "OK! The password has both big letters and special characters !");
            }
            else
            {
                QMessageBox::information(this, "Message", "OK! The password has only big letters !");
            }
        }
        else if(ui->checkBox_special_ctrl->isChecked() && nesne.Control_Special(password_ctrl))
        {
            if(ui->checkBox_number_ctrl->isChecked() && nesne.Control_Number(password_ctrl))
            {
                QMessageBox::information(this, "Message", "OK! The password has numbers and special characters !");
            }
            else
            {
                QMessageBox::information(this, "Message", "OK! The password has only special characters !");
            }
        }
        else if(ui->checkBox_number_ctrl->isChecked() || ui->checkBox_letter_ctrl->isChecked() || ui->checkBox_special_ctrl->isChecked())
        {
            QMessageBox::critical(this, "Message", "The password does not provide given properties !");
        }
    }
    else
    {           //if "else" does not exists, program will give error for all the case's
        QMessageBox::warning(this, "Character bound", "The password must be in the range of 8-20 characters !");
    }
}



void manager::on_pushButton_save_clicked()
{
    {
        QString username, password,source, key = "";
        unsigned char* u_key = NULL;
//        connOpen();
        passwdsource dialog;                        //create instance from passwdsource for use to passwdsource's function
        dialog.setModal(true);                      //new dialog opens and getting source and username for given password..
        if(dialog.exec() == QDialog::Accepted)      //proceed when press OK
        {
            username = dialog.getUsername();
            source = dialog.getSource();
            password = ui->lineEdit->text();

            if(username != "" && source != "" && password != "")        //if given lines are not empty
            {
                if(!isconnOpened())
                {
                    qDebug() << "Failed to open the database";
                    return;
                }
                QSqlQuery qry;                                          //query object....below code is the sql query......to sending database...
                QString crypt_user, crypt_passwd, crypt_source = "";
                EncryptDecrypt instance;
                key = UserKey;
                key = instance.decrypt_it(key, key.length()/2, instance.getMasterKey());
                u_key = (unsigned char*)StringToChar(key);
                crypt_user   = instance.encrypt_it(username, u_key);     //username crypted
                crypt_passwd = instance.encrypt_it(password, u_key);     //password crypted
                crypt_source = instance.encrypt_it(source, u_key);       //source crypted

                qry.prepare("insert into data (username, password, source, key) values('"+crypt_user+"', '"+crypt_passwd+"', '"+crypt_source+"', '"+UserKey+"')");
                if(qry.exec())
                {
                    QMessageBox::information(this, tr("Save"), tr("Saved !"));                    //if process executed successfully
                }
                else
                {
                    QMessageBox::information(this, tr("error::"), qry.lastError().text());        //if an error occures, throw this error (exception..)
                }
            }
            else
            {
                QMessageBox::information(this, tr("Error"), tr("Please fill in the blanks !"));
            }
        }
        free(u_key);
    }
//    connClose();
}



void manager::on_pushButton_ctrl_save_clicked()
{
    {
        QString username, password,source, key = "";
        unsigned char* u_key = NULL;
//        connOpen();
        passwdsource dialog;                        //create instance from passwdsource for use to passwdsource's function
        dialog.setModal(true);                      //new dialog opens and getting source and username for given password..
        if(dialog.exec() == QDialog::Accepted)      //proceed when press OK
        {
            username = dialog.getUsername();
            source = dialog.getSource();
            password = ui->lineEdit_ctrl->text();

            if(username != "" && source != "" && password != "")        //if given lines are not empty
            {
                if(!isconnOpened())
                {
                    qDebug() << "Failed to open the database";
                    return;
                }
                QSqlQuery qry;                                          //query object....below code is the sql query......to sending database...
                QString crypt_user, crypt_passwd, crypt_source = "";
                EncryptDecrypt instance;
                key = UserKey;
                key = instance.decrypt_it(key, key.length()/2, instance.getMasterKey());
                u_key = (unsigned char*)StringToChar(key);
                crypt_user   = instance.encrypt_it(username, u_key);      //username crypted
                crypt_passwd = instance.encrypt_it(password, u_key);      //password crypted
                crypt_source = instance.encrypt_it(source, u_key);        //source crypted

                qry.prepare("insert into data (username, password, source, key) values('"+crypt_user+"', '"+crypt_passwd+"', '"+crypt_source+"', '"+UserKey+"')");

                if(qry.exec())
                {
                    QMessageBox::information(this, tr("Save"), tr("Saved !"));                    //if process executed successfully
                }
                else
                {
                    QMessageBox::information(this, tr("error::"), qry.lastError().text());        //if an error occures, throw this error (exception..)
                }
            }
            else
            {
                QMessageBox::information(this, tr("Error"), tr("Please fill in the blanks !"));
            }
        }
        free(u_key);
    }
//    connClose();
}



void manager::on_pushButton_show_clicked()
{                                               //password should be showed as plain text..
    {                                           //taking all the list from db then showing
        EncryptDecrypt instance;                //records by their id's one by one, the
                                                //list is not directly retrieved from the
                                                //db and displayed. there is decryption process

        QString user_e, passwd_e, source_e;     //encrypted texts
        QString user_d, source_d;               //decrypted texts
        QString key, id;
        unsigned char* u_key = NULL;
//        connOpen();
        QSqlQuery* qry = new QSqlQuery(mydb);

        int column = 4;
        int last_id = 0;                            //id number of the last record in the database
        int row_count = 0;                          //total filled row number in the db..According to it, the number of rows will be given and it will enter the loop.
        int row_current = 0;                        //to assign process clean, current row index is

        qry->prepare("select * from data order by id desc limit 1");
        qry->exec();
        if(qry->next())
            last_id = qry->value(0).toInt();                        //last id number in the database

        for(int i=1; i<=last_id; i++)                               //here, controlling filled rows
        {                                                           //to clean table view (5 filled rows, 5 rows should be in table)
            qry->prepare("select * from data where id = :id");
            qry->bindValue(":id", i);
            qry->exec();
            if(qry->next() && qry->value(4) == UserKey)             //if keys are matched
            {                                                       //increase row number to show
                row_count++;
            }
        }
        QStringList labels;
        labels << tr("Username") << tr("Password") << tr("Source");
        ui->tableWidget->setColumnCount(column);
        ui->tableWidget->setRowCount(row_count);
        ui->tableWidget->setHorizontalHeaderLabels(labels);
        ui->tableWidget->QTableView::setColumnHidden(3, true);                              //column id is hide now..
        ui->tableWidget->QTableView::setEditTriggers(QAbstractItemView::NoEditTriggers);    //cells can not be editable by this line.
        if(row_count != 0)
        {
            for(int i=1; i<=last_id; i++)
            {
                qry->prepare("select * from data where id = :id");
                qry->bindValue(":id", i);
                qry->exec();
                if(qry->next() && qry->value(4) == UserKey)                             //enter inside this if, by the matched keys and also filled rows
                {
                    user_e   = qry->value(1).toString();
                    passwd_e = qry->value(2).toString();
                    source_e = qry->value(3).toString();
                    id       = qry->value(0).toString();                                //get id from database
                    key      = UserKey;
                    key      = instance.decrypt_it(key, key.length()/2, instance.getMasterKey());
                    u_key    = (unsigned char*)StringToChar(key);
                    user_d   = instance.decrypt_it(user_e, user_e.length()/2, u_key);
//                  passwd_d = instance.decrypt_it(passwd_e, passwd_e.length()/2, u_key);     //there is no needed decryption because passwords are showed as **** for the first time,,passwords decrypted in cellClicked() function
                    source_d = instance.decrypt_it(source_e, source_e.length()/2, u_key);
                    ui->tableWidget->setItem(row_current,0, new QTableWidgetItem(user_d));
                    ui->tableWidget->setItem(row_current,1, new QTableWidgetItem("********"));
                    ui->tableWidget->setItem(row_current,2, new QTableWidgetItem(source_d));
                    ui->tableWidget->setItem(row_current,3, new QTableWidgetItem(id));          //insert id number exactly the same as db and store in hided column
                    row_current++;                                                              //insert element with respect to number of valid records
                }
            }
        }
        free(u_key);
    }
//    connClose();
}



void manager::on_pushButton_delete_clicked()
{
    bool control=false;
    {
        int sel_row_num = 0, id = 0;
        QString id_str;
//        connOpen();
        QSqlQuery* qry = new QSqlQuery(mydb);

        QModelIndexList indexes = ui->tableWidget->selectionModel()->selectedRows();
        if( indexes.count() > 0 )                                             //if there is no selected row, function pass this
        {
            sel_row_num = indexes[0].row();                                   // will return the row-index of the first selected row

            id_str = ui->tableWidget->item(sel_row_num,3)->text();            // get the id of selected row, then delete in the database
            id = id_str.split(" ")[0].toInt();                                // get id which is exactly the same as db, through to hided row (stores id number same as in the database)
            qry->prepare("delete from data where id = :id");
            qry->bindValue(":id", id);
            if(qry->exec())
            {
                QMessageBox::information(this, tr("Delete"), tr("Deleted!"));
                control = true;
            }
            else
            {
                QMessageBox::critical(this, tr("error::"), qry->lastError().text());
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), tr("Please select a row to be deleted!"));
        }
        delete qry;
    }
//    connClose();
    if(control)
        on_pushButton_show_clicked();
}



void manager::on_pushButton_deleteUser_clicked()
{                                                                                               //delete all user records
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete User",
                                  "Are you sure about delete existing user ?<br><br>"
                                  "Note that, this will delete all the passwords of this user.",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        qDebug() << "Yes was clicked";
//        connOpen();
        QSqlQuery* qry = new QSqlQuery(mydb);
        qry->prepare("delete from data where key = :key");
        qry->bindValue(":key", UserKey);
        if(qry->exec())
        {
            qry->prepare("delete from logindata where key = :key");
            qry->bindValue(":key", UserKey);
            if(qry->exec())
            {
                QMessageBox::information(this, tr("Delete"), tr("This user was deleted !"));
                QDialog::close();
                connClose();
                login *dialog = new login();
                dialog->show();
            }
        }
    }
    else
    {
        qDebug() << "Yes was *not* clicked";
    }
//    connClose();
}



void manager::on_pushButton_deleteUser_2_clicked()
{
    on_pushButton_deleteUser_clicked();
}



//The following functions check if the options
//in the selected check boxes are actually included
//in the password. Since the password is generated
//with the character set given to it, these characters
//sometimes may not be in the generated password, so
//these passwords should be checked.

bool letter_special_number(QString password)
{
    QString str_letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString str_special = ".,!'#+%-_";
    QString str_number = "0123456789";

    for(int i=0; i<=str_letter.length(); i++)
    {
        if(password.contains(str_letter[i]))
        {
            for(int j=0; j<=str_special.length(); j++)
            {
                if(password.contains(str_special[j]))
                {
                    for(int k=0; k<=str_number; k++)
                    {
                        if(password.contains(str_number[k]))
                        {
                            return true;
                        }
                    }
                    return false;
                }
            }
            return false;
        }
    }
    return false;
}


bool letter_special(QString password)
{
    QString str_letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString str_special = ".,!'#+%-_";

    for(int i=0; i<=str_letter.length(); i++)
    {
        if(password.contains(str_letter[i]))
        {
            for(int j=0; j<=str_special.length(); j++)
            {
                if(password.contains(str_special[j]))
                {
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}


bool letter_number(QString password)
{
    QString str_letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString str_number = "0123456789";

    for(int i=0; i<=str_letter.length(); i++)
    {
        if(password.contains(str_letter[i]))
        {
            for(int j=0; j<=str_number.length(); j++)
            {
                if(password.contains(str_number[j]))
                {
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}


bool special_number(QString password)
{
    QString str_special = ".,!'#+%-_";
    QString str_number = "0123456789";

    for(int i=0; i<=str_special.length(); i++)
    {
        if(password.contains(str_special[i]))
        {
            for(int j=0; j<=str_number.length(); j++)
            {
                if(password.contains(str_number[j]))
                {
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}


bool letter(QString password)
{
    QString str_letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for(int i=0; i<=str_letter.length(); i++)
    {
        if(password.contains(str_letter[i]))
        {
            return true;
        }
    }
    return false;
}


bool special(QString password)
{
    QString str_special = ".,!'#+%-_";

    for(int i=0; i<=str_special.length(); i++)
    {
        if(password.contains(str_special[i]))
        {
            return true;
        }
    }
    return false;
}


bool number(QString password)
{
    QString str_number = "0123456789";

    for(int i=0; i<=str_number.length(); i++)
    {
        if(password.contains(str_number[i]))
        {
            return true;
        }
    }
    return false;
}




