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
    int gameId = ui->comboBox_game->currentIndex()+1;
    write(conn->sd, &gameId, sizeof(int));
    this->close();
}
