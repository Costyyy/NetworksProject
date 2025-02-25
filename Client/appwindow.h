#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QDialog>
#include "serverconnection.h"
namespace Ui {
class AppWindow;
}

class AppWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AppWindow(QWidget *parent = nullptr);
    ~AppWindow();
    serverConnection *conn;
private slots:
    void on_pushButton_disconnect_clicked();


    void on_pushButton_test_clicked();

private:
    Ui::AppWindow *ui;
};

#endif // APPWINDOW_H
