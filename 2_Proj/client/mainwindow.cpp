#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    /* create instance */
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowMyself()
{
  this->show();
}


void MainWindow::on_pushButton_tcpSetting_clicked()
{
    this->hide();
    emit show_tcpClient();
}

void MainWindow::on_pushButton_music_clicked()
{
    this->hide();
    emit show_musicPlayer();
}

void MainWindow::on_pushButton_main_clicked()
{
    ShowMyself();
}

