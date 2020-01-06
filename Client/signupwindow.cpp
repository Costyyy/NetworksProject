#include "signupwindow.h"
#include "ui_signupwindow.h"
#include <QMessageBox>
signupwindow::signupwindow(QWidget *parent, serverConnection *connection) :
    QDialog(parent),
    conn(connection),
    ui(new Ui::signupwindow)
{
    ui->setupUi(this);
    QStatusBar *bar = new QStatusBar(this);
    ui->verticalLayout_status->addWidget(bar);
}

signupwindow::~signupwindow()
{
    delete ui;
}

struct signupData
{
    char username[100];
    char password[100];
};

void signupwindow::on_pushButton_done_clicked()
{
    signupData *data = new signupData;
    if(ui->lineEdit_username->text().size() < 4)
    {

    }
    else if(ui->lineEdit_password->text().size() < 4)
    {

    }
    else if(ui->lineEdit_password->text() != ui->lineEdit_rpass->text())
    {

    }
    else
    {
        int mess = 0;
        strcpy(data->username, ui->lineEdit_username->text().toStdString().c_str());
        strcpy(data->password, ui->lineEdit_password->text().toStdString().c_str());
        write(conn->sd, &mess, sizeof(int));

        write(conn->sd, data, sizeof(signupData));

        read(conn->sd, &mess, sizeof(int));
        if(!mess)
        {
            QMessageBox::information(this,"Result", "Account created succesfully!");
        }
        else
        {
            QMessageBox::warning(this,"Result", "This username already exists!");
        }
    }
}
