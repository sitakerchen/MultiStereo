#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    /* create instance */
    ui->setupUi(this);
    m_client = new tcpclient;

    }

MainWindow::~MainWindow()
{
    delete ui;
    delete m_client;
}


void MainWindow::on_pushButton_tcpSetting_clicked()
{
    m_client->show();
}


void MainWindow::on_pushButton_music_clicked()
{

}

void MainWindow::on_pushButton_main_clicked()
{

}

