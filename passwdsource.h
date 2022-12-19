#ifndef PASSWDSOURCE_H
#define PASSWDSOURCE_H

#include <QDialog>
#include <QString>

namespace Ui {
class passwdsource;
}

class passwdsource : public QDialog
{
    Q_OBJECT

public:
    explicit passwdsource(QWidget *parent = nullptr);
    ~passwdsource();
    QString getSource();
    QString getUsername();

private slots:
    void on_pushButton_clicked();

private:
    Ui::passwdsource *ui;
};

#endif // PASSWDSOURCE_H
