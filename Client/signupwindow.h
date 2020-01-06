#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QDialog>
#include <serverconnection.h>
#include <QtWidgets/QStatusBar>
namespace Ui {
class signupwindow;
}

class signupwindow : public QDialog
{
    Q_OBJECT

public:
    explicit signupwindow(QWidget *parent = nullptr, serverConnection* = nullptr);
    serverConnection *conn;
    ~signupwindow();

private slots:
    void on_pushButton_done_clicked();

private:
    Ui::signupwindow *ui;
};

#endif // SIGNUPWINDOW_H
