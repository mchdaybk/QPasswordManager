#include "passwdsource.h"
#include "ui_passwdsource.h"
#include <QTextLayout>

passwdsource::passwdsource(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::passwdsource)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    ui->lineEdit_username->setPlaceholderText("What is the username for this password?");
    ui->lineEdit->setPlaceholderText("Where do you want to use this password? (twitter, facebook etc.)");
    setWindowIcon(QIcon("C:/Users/msaybek/Desktop/repository/QPasswordManager/key.png"));
    setWindowTitle("Username and Source");
}

passwdsource::~passwdsource()
{
    delete ui;
}

QString passwdsource::getSource()
{
    QString source = ui->lineEdit->text();
    return source;
}

QString passwdsource::getUsername()
{
    QString username = ui->lineEdit_username->text();
    return username;
}

void passwdsource::on_pushButton_clicked()
{
    QDialog::close();                                               //hide yerine dialog kapatıldı!!!!!!!!!SON DEGISIKLIKLERDEN
    //hide();
    //buraya bir bool return vs konularak, kayit iptal edilmesi
    //durumunda yazilanlari kaydetmeme durumu dusunulebilir.
    //suan parola kaydet denildikten sonra herhangi bir metin girilmese de kaydediyor.

    //ui->setupUi ALTINA YAZILAN SAYESİNDE BU BAHSEDİLEN İŞLEM TAMAMLANDI..
}

