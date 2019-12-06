#ifndef CREATEGAME_H
#define CREATEGAME_H

#include <QDialog>
#include "appwindow.h"
#include "serverconnection.h"
namespace Ui {
class CreateGame;
}

class CreateGame : public QDialog
{
    Q_OBJECT

public:
    explicit CreateGame(QWidget *parent = nullptr, serverConnection* = nullptr);
    ~CreateGame();
    serverConnection *conn;
private slots:
    void on_pushButton_finish_clicked();

private:
    Ui::CreateGame *ui;
    QWidget *prnt;
};

#endif // CREATEGAME_H
