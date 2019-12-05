#include "appwindow.h"
#include "ui_appwindow.h"
AppWindow::AppWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppWindow)
{
    ui->setupUi(this);
}

AppWindow::~AppWindow()
{
    delete ui;
}

void AppWindow::on_pushButton_disconnect_clicked()
{
    int mess = 0;
    if(write(this->conn->sd, &mess, sizeof(int)) <= 0)
    {
        printf("error at write dc\n");
    }
    this->close();
}

void AppWindow::on_pushButton_test_clicked()
{
    int mess = 1;
    ui->listWidget_test->addItem("test");
    if(write(this->conn->sd, &mess, sizeof(int)) <= 0)
    {
        printf("error at write dc\n");
    }
}

void AppWindow::on_pushButton_refresh_clicked()
{
    int mess = 2;
    ui->listWidget_test->addItem("test");
    if(write(this->conn->sd, &mess, sizeof(int)) <= 0)
    {
        printf("error at write dc\n");
    }

}
