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

void homePage::ins_process(QString ins)
{
    qint64 uACt_obj, uAct_name, uAct_val;
    QString qstrMsg_error;
    codecodeSys::decode_act(ins, uACt_obj, uAct_name, uAct_val, qstrMsg_error);
    if (uAct_name == ACT_NAME_ASSIGN_ID)
    {
        setId(uAct_val);
    }
}

void homePage::setId(qint64 id)
{
    ui->label_id->setScaledContents(true);
    ui->label_id->setText(QString::number(id));
}

void homePage::ShowMyself()
{
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

