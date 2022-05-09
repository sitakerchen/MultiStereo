#include "homePage.h"
#include "ui_homePage.h"

homePage::homePage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::homePage)
{
    /* create instance */
    ui->setupUi(this);
}

homePage::~homePage()
{
    delete ui;
}

void homePage::ins_process(qint64 uAct_name, QString uAct_val)
{
    if (uAct_name == ACT_NAME_ASSIGN_ID)
    {
        setId(uAct_val.toInt());
    }
}

void homePage::setId(qint64 id)
{
    ui->label_id->setText(QString::number(id));
}

void homePage::setStatus(bool stat)
{
    if (stat) // connected status
    {
        ui->label_status->setText("已连接");
        ui->label_status->setStyleSheet("background-color: rgba(0, 255, 0, 1);");
    }
    else
    {
        ui->label_status->setText("未连接");
        ui->label_status->setStyleSheet("background-color: rgba(255, 0, 0, 1);");
        setChannel("未分配");
    }
}

void homePage::setChannel(QString channel)
{
    ui->label_channel->setText(channel);
}

void homePage::ShowMyself()
{
    ui->pushButton_main->setIcon(QIcon(":/image/image/image-selected/24gl-home2-red.png"));
    this->show();
}


void homePage::on_pushButton_tcpSetting_clicked()
{
    this->hide();
    emit show_tcpClient();
}

void homePage::on_pushButton_music_clicked()
{
    this->hide();
    emit show_musicPlayer();
}

void homePage::on_pushButton_main_clicked()
{
    ShowMyself();
}

