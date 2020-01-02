#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include "serverconnection.h"
namespace Ui {
class GameWindow;
}

class GameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr, serverConnection* = nullptr);
    serverConnection *conn;
    ~GameWindow();

private slots:

    void on_pushButton_submit_clicked();

private:
    Ui::GameWindow *ui;
    static void *gameLoop(void *arg);
    pthread_t th;
};

#endif // GAMEWINDOW_H
