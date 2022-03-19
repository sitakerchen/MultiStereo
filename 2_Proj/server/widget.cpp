#include "widget.h"
#include "tcpserver.h"
#include "ui_widget.h"
#include "processcontrol.h"
#include "macro.h"
using Qt::endl; using PC = ProcessControl;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // set currentDir to application Dir(binary executable file)
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    PC::getInstance().setWorkingPath(); // set terminal/cmd's current path

    /* initiate widget*/
    //    widget that can't be toggled before select stereoSound
//    ui->pushButtonShowTargetInfo->setEnabled(false); // abandon temporarily
    ui->pushButtonSplit2mono->setEnabled(false);
    ui->pushButtonTransmit->setEnabled(false);

    /* create TcpServer */
    m_ui_server = new tcpserver;

    /* connect */
    connect(&PC::getInstance(), &PC::sendErrorInfo, this, &Widget::displayErrorInfo);
    connect(&PC::getInstance(), &PC::sendOutPutInfo, this, &Widget::displayOutPutInfo);
}

Widget::~Widget()
{
    delete ui;
}

// choose 1 stereo music file
void Widget::on_pushButtonChoose1stereo_clicked()
{
    m_urlAudioFile = QFileDialog::getOpenFileUrl(this, "open Audio File", QUrl(), "WAV MP3 files(*.mp3 *.wav)");
    if ( ! m_urlAudioFile.isEmpty() )
    {
        // print the url-Link-Path of AudioFile
        qDebug() << "m_urlAudioFile path: " << m_urlAudioFile.path() << Qt::endl;
//        ui->pushButtonShowTargetInfo->setEnabled(true); // abandon temporarily
        ui->pushButtonSplit2mono->setEnabled(true);
    }
}

// split stereo file into seperate channel (split into several files)
void Widget::on_pushButtonSplit2mono_clicked()
{
    if ( m_urlAudioFile.isValid() == false)
    {
        qDebug() << " url File is invalid " << Qt::endl;
        return ;
    }
    QString qstrCmd = FFMPEG_SPLIT_2.arg( m_urlAudioFile.fileName() );
    qDebug() << "qstrCmd" << qstrCmd << endl;
    PC::getInstance().writeCommand(qstrCmd);
    ui->pushButtonTransmit->setEnabled(true);
}

// transmit data to client(mobile device)
void Widget::on_pushButtonTransmit_clicked()
{
    m_ui_server->show();
}

// display process std error infomation in textBrowser
void Widget::displayErrorInfo(QString msg)
{
    ui->textBrowserError->append(msg);
}

// display process std outPut  info in textBrowser
void Widget::displayOutPutInfo(QString msg)
{
    ui->textBrowserOutPut->append(msg);
}

// show info of chosen stereo file (some bugs, ignore this function temporary)
//void Widget::on_pushButtonShowTargetInfo_clicked()
//{
//     if (m_urlAudioFile.isValid() == false)
//    {
//        qDebug() << tr("url File is invalid") << Qt::endl;
//        return ;
//    }
//    QString qstrCmd = tr("ffprobe -i %1\r\n");
////    qstrCmd = qstrCmd.arg(m_urlAudioFile.path().mid(1).replace('/', '\\'));
//    qstrCmd = qstrCmd.arg( QDir::toNativeSeparators(m_urlAudioFile.path().mid(1)) ); // mid(1): remove prefix '/...'
//    std::string strTemp = qstrCmd.toStdString();
//    const char *szCmd = strTemp.c_str();

//    // debug

//    qDebug() << "szCmd : " << szCmd << Qt::endl;
//    // debug

//    m_process->write(szCmd);
//    QMessageBox::information(this, tr("Info"), m_process->readAllStandardOutput().data());
//}

