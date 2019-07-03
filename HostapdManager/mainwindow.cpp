#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hostapdcontroller.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    hostapdController(new HostapdController())
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete hostapdController;
}

void MainWindow::on_btn_reset_clicked()
{
    ui->edit_wifi_ssid->clear();
    ui->edit_wifi_passwd->clear();
    ui->edit_show_log->clear();
    ui->edit_interface->clear();
    ui->edit_drive_type->clear();

    // Stop
    on_btn_stop_clicked();
}

void MainWindow::on_btn_start_clicked()
{
    QString log;
    if(ui->edit_wifi_ssid->text().trimmed().length() == 0) {
        log = QString("WiFi Ssid cannot be empty!");
    } else {
        log = QString("Staring..... Ssid:%1, Passwd:%2").arg(ui->edit_wifi_ssid->text()).arg(ui->edit_wifi_passwd->text());
    }
    OnLogUpdate(log);

    bool rt = hostapdController->GenerateConfig(ui->edit_wifi_ssid->text(), ui->edit_wifi_passwd->text(), ui->edit_interface->text(), ui->edit_drive_type->text());
    if(rt) {
        hostapdController->RunHostapd();
    }
}

void MainWindow::on_btn_stop_clicked()
{
    OnLogUpdate("Stop......");
    hostapdController->StopHostapd();
}

void MainWindow::OnLogUpdate(const QString &newLog) {
    QString text = ui->edit_show_log->toPlainText() + newLog + "\r\n";
    ui->edit_show_log->setText(text);

    // Update cursor to end
    auto cursor = ui->edit_show_log->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->edit_show_log->setTextCursor(cursor);
}
