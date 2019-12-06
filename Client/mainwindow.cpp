#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serverconnection.h"
#include <QMessageBox>
#include <QtDebug>
#include <string>
#include <iostream>
MainWindow::MainWindow(QWidget *parent, serverConnection *connection)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    conn(connection)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

struct tempInfo
{
    char username[100];
    char password[100];
};

void MainWindow::on_pushButton_login_clicked()
{

    tempInfo *info = new tempInfo;
    strcpy(info->username, ui->lineEdit_username->text().toStdString().c_str());
    strcpy(info->password, ui->lineEdit_password->text().toStdString().c_str());
    int res;
    if(ui->lineEdit_username->text().toStdString().empty() or ui->lineEdit_password->text().toStdString().empty())
    {
        ui->statusbar->showMessage("Username or password cannot be empty!");
        return;
    }
    std::cout << info->username << "\n" << info->password << '\n';
    if (write(conn->sd, info, sizeof(tempInfo)) <= 0)
    {
        perror("[client]Eroare la write() spre server.\n");
        ui->statusbar->showMessage("Cannot connect to server! Please check your connection!");
        return;
    }
    if (read(conn->sd, &res, sizeof(int)) < 0)
    {
        perror("[client]Eroare la read() de la server.\n");
    }
    /* afisam mesajul primit */
    if(res)
    {
        //QMessageBox::information(this,"Login","Login succesful");
        ui->statusbar->showMessage("Login succesful!");
        appWindow = new AppWindow(this);
        appWindow->conn = conn;
        appWindow->show();
    }
    else
    {
        //QMessageBox::warning(this,"Login","Username or password invalid");
        ui->statusbar->showMessage("Login failed!");
    }

}

