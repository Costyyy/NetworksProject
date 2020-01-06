#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <pthread.h>

struct info
{
    void *arg1;
    void *arg2;
    void *arg3;
};
GameWindow::GameWindow(QWidget *parent, serverConnection *connection) :
    QDialog(parent),
    conn(connection),
    parent(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    ui->label_question->setText("Waiting for the game to start...");
    ui->radioButton->setVisible(false);
    ui->radioButton_2->setVisible(false);
    ui->radioButton_3->setVisible(false);
    ui->radioButton_4->setVisible(false);
    ui->pushButton_submit->setVisible(false);
    ui->pushButton_finish->setVisible(false);

    info *arg = new info;
    arg->arg1 = conn;
    arg->arg2 = ui;
    arg->arg3 = this;
    pthread_create(&th, nullptr, gameLoop, arg);

}
struct questions
{
    char text[256];
    char answ[4][128];
};
int picked[4];
void *GameWindow::gameLoop(void *arg)
{
    info *dates = (info *)arg;
    serverConnection *conn = (serverConnection *) (dates->arg1);
    Ui::GameWindow *ui = (Ui::GameWindow *)(dates->arg2);
    GameWindow *win = (GameWindow *)(dates->arg3);
    int msg;
    questions *question = new questions;
    QRadioButton *buttons[4];
    srand(time(NULL));
    buttons[0] = ui->radioButton;
    buttons[1] = ui->radioButton_2;
    buttons[2] = ui->radioButton_3;
    buttons[3] = ui->radioButton_4;
    while(1)
    {
        read(conn->sd, &msg, sizeof(int));
        if(msg == 1)
        {
            for(int i = 0; i < 4; i++)
            {
                buttons[i]->setVisible(true);
            }
            ui->pushButton_submit->setVisible(true);
            //update
            read(conn->sd, question, sizeof(questions));
            ui->label_question->setText(question->text);

            for(int i = 0; i < 4; i++)
            {
                buttons[i]->setVisible(true);
            }
            for(int i = 0; i < 4; i++)
            {
                picked[i] = -1;
            }
            int randNo;
            for(int i = 0; i < 4; i++)
            {
                randNo = rand()%4;
                while(picked[randNo] != -1)
                {
                    randNo = rand()%4;
                }
                buttons[i]->setText(question->answ[randNo]);
                picked[randNo] = i;
            }

        }
        else if(msg == 2)
        {
            //close thread
            for(int i = 0; i < 4; i++)
            {
                buttons[i]->setVisible(false);
            }
            ui->pushButton_submit->setVisible(false);

            printf("msg:%i\n", msg);
            char buff[256];
            char endText[512];
            read(conn->sd, buff, sizeof(buff));
            sprintf(endText, "%s has won!", buff);
            printf("msg:%s\n", endText);
            ui->label_question->setText(endText);
            ui->pushButton_finish->setVisible(true);
            return nullptr;
        }
        else if(msg == 3)
        {
            ui->label_question->setVisible(true);

            ui->label_question->setText("Waiting For Turn...");
        }
        else if(msg == 4)
        {
            for(int i = 0; i < 4; i++)
            {
                buttons[i]->setVisible(false);
            }
            ui->label_question->setText("Correct!");
        }
        else if(msg == 0)
        {
            for(int i = 0; i < 4; i++)
            {
                buttons[i]->setVisible(false);
            }
            ui->label_question->setText("Incorrect! You have been eliminated!");
        }
        else
        {
            continue;
        }
    }

}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::on_pushButton_submit_clicked()
{
    int answer;
    if(ui->radioButton->isChecked())
    {

        for(int i = 0; i < 4; i++)
        {
            if(picked[i] == 0)
            {
                answer = i;
            }
        }
    }
    else if(ui->radioButton_2->isChecked())
    {
        for(int i = 0; i < 4; i++)
        {
            if(picked[i] == 1)
            {
                answer = i;
            }
        }
    }
    else if(ui->radioButton_3->isChecked())
    {
        for(int i = 0; i < 4; i++)
        {
            if(picked[i] == 2)
            {
                answer = i;
            }
        }
    }
    else if(ui->radioButton_4->isChecked())
    {
        for(int i = 0; i < 4; i++)
        {
            if(picked[i] == 3)
            {
                answer = i;
            }
        }
    }
    else
    {
        return;
    }
    printf("answ:%i\n", answer);
    write(conn->sd, &answer, sizeof(int));
}

void GameWindow::on_pushButton_finish_clicked()
{
    parent->show();
    this->setModal(false);
    this->close();
}
