#include "creategame.h"
#include "ui_creategame.h"


CreateGame::CreateGame(QWidget *parent, serverConnection *connection) :
    QDialog(parent),
    conn(connection),
    ui(new Ui::CreateGame)
{
    ui->setupUi(this);
}

CreateGame::~CreateGame()
{
    delete ui;
}
struct gameData
{
    int gameId;
};

void CreateGame::on_pushButton_finish_clicked()
{
    int playerCount;
    switch (ui->comboBox_game->currentIndex()) {
    case 0:
        playerCount = 2;
        break;
    case 1:
        playerCount = 5;
        break;
    case 2:
        playerCount = 10;
        break;
    }
    write(conn->sd, &playerCount, sizeof(int));
    this->close();
}
