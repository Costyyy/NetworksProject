#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serverconnection.h"
#include <QMessageBox>
#include <QtDebug>
#include <string>
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_login_clicked()
{
    serverConnection *conn = new serverConnection;
    const char* ip = "127.0.0.1";
    conn->connectTo(ip);
    char username[100];
    char password[100];
    strcpy(username, ui->lineEdit_username->text().toStdString().c_str());
    strcpy(password, ui->lineEdit_password->text().toStdString().c_str());
    char info[100];
    int res;
    sprintf(info, "%s:%s", username, password);

    if (write(conn->sd, info, sizeof(info)) <= 0)
    {
        perror("[client]Eroare la write() spre server.\n");
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

