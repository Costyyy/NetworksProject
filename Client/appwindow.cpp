#include "appwindow.h"
#include "ui_appwindow.h"
#include "creategame.h"
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

struct GameData
{
    char user[100][50];
    int gameId[50];
    int count = 0;
};

void AppWindow::on_pushButton_disconnect_clicked()
{
    int mess = 0;
    if(write(this->conn->sd, &mess, sizeof(int)) <= 0)
    {
        printf("error at write dc\n");
    }
    this->close();
}

void AppWindow::on_pushButton_refresh_clicked()
{

    int mess = 2;
    GameData *gData = new GameData;
    if(write(this->conn->sd, &mess, sizeof(int)) <= 0)
    {
        printf("error at write dc\n");
    }

    if(read(this->conn->sd, gData, sizeof(GameData)) <= 0)
    {
        printf("error at read gameData\n");
    }
    ui->listWidget_test->clear();
    char matchStringData[256];
    for(int i = 0; i < gData->count; i++)
    {
        switch(gData->gameId[i])
        {
        case 1:
            sprintf(matchStringData, "%s-%s", gData->user[i],"CS:GO");
            break;
        case 2:
            sprintf(matchStringData, "%s-%s", gData->user[i],"SMITE");
            break;
        }


        ui->listWidget_test->addItem(matchStringData);
    }

}




void AppWindow::on_pushButton_create_clicked()
{
    int mess = 3;
    if(write(this->conn->sd, &mess, sizeof(int)) <= 0)
    {
        printf("error at write dc\n");
    }
    CreateGame *gameWindow = new CreateGame(this, conn);
    gameWindow->show();
}

