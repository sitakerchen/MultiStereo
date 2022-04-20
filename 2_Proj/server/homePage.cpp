#include "homePage.h"
#include "ui_homePage.h"
using Qt::endl; using PC = ProcessControl;

homePage::homePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::homePage)
{
    ui->setupUi(this);
    this->setWindowTitle("Main Window");

    /* set currentDir to application Dir(binary executable file) */
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    PC::getInstance().setWorkingPath(); // set terminal/cmd's current path

    /* create tcpController and mediaProcess*/
    m_tcpCtr = new tcpController;
    m_MedaProces = new rawMediaProcess;

    /* connect */
    connect(m_tcpCtr, &tcpController::evoke_split, m_MedaProces, &rawMediaProcess::split_2);
    connect(m_MedaProces, &rawMediaProcess::evoke_music_synchronization, m_tcpCtr, &tcpController::synchronize_musicFile);
}

homePage::~homePage()
{
    delete ui;
    if (m_tcpCtr)
    {
        delete m_tcpCtr;
        m_tcpCtr = nullptr;
    }
    if (m_MedaProces)
    {
        delete m_MedaProces;
        m_MedaProces = nullptr;
    }
}

void homePage::on_pushButtonMediaProcess_clicked()
{
    m_MedaProces->show();
}


void homePage::on_pushButtonController_clicked()
{
    m_tcpCtr->show();
}


void homePage::on_pushButtonChangeLayout_clicked()
{
    qint64 nCurIndex = ui->stackedWidget->currentIndex();
    if (nCurIndex == 0)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

