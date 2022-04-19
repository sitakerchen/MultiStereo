#include "rawmediaprocess.h"
#include "ui_rawmediaprocess.h"
using Qt::endl; using PC = ProcessControl;

rawMediaProcess::rawMediaProcess(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::rawMediaProcess)
{
    ui->setupUi(this);

    /* connect to ffmpeg process */
    connect(&PC::getInstance(), &PC::sendErrorInfo, this, &rawMediaProcess::displayErrorInfo);
    connect(&PC::getInstance(), &PC::sendOutPutInfo, this, &rawMediaProcess::displayOutPutInfo);
}

rawMediaProcess::~rawMediaProcess()
{
    delete ui;
}

void rawMediaProcess::on_pushButtonChoose1stereo_clicked()
{
    m_urlAudioFile = QFileDialog::getOpenFileUrl(this, "open Audio File", QUrl(), "WAV MP3 files(*.mp3 *.wav)");
    if ( ! m_urlAudioFile.isEmpty() )
    {
        qDebug() << "m_urlAudioFile path: " << m_urlAudioFile.path() << Qt::endl; // print the url-Link-Path of AudioFile
        ui->pushButtonSplit2mono->setEnabled(true);
    }
}

void rawMediaProcess::on_pushButtonSplit2mono_clicked()
{
    if ( m_urlAudioFile.isValid() == false)
    {
        qDebug() << " url File is invalid " << Qt::endl;
        return ;
    }
    QString qstrCmd = FFMPEG_SPLIT_2.arg( m_urlAudioFile.fileName() );
    qDebug() << "qstrCmd" << qstrCmd << endl;
    PC::getInstance().writeCommand(qstrCmd);
}

void rawMediaProcess::displayErrorInfo(QString msg)
{
    ui->textBrowserError->append(msg);
}

void rawMediaProcess::displayOutPutInfo(QString msg)
{
    ui->textBrowserOutPut->append(msg);
}

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



void rawMediaProcess::on_pushButton_writeCmd_clicked()
{
    QString qstrCmd = ui->lineEdit_cmdInput->text();
    PC::getInstance().writeCommand(qstrCmd);
}

