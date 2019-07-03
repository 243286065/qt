#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class HostapdController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_reset_clicked();

    void on_btn_start_clicked();

    void on_btn_stop_clicked();

    void OnLogUpdate(const QString &newLog);

private:
    Ui::MainWindow *ui;
    HostapdController *hostapdController;
};

#endif // MAINWINDOW_H
