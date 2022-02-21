#include "widget.h"
#include "ui_widget.h"

#ifdef Q_OS_WIN
const QString ROOT_OF_FILE = "D:";
#else
const QString ROOT_OF_FILE = "/";
#endif

using ll = long long;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //    m_process->setProcessChannelMode(QProcess::MergedChannels);

    /* initiate cmd/terminal process */
    m_process = new QProcess(this);
#ifdef Q_OS_WIN
    m_process->start("cmd");
#else
    m_process->start("bash");
#endif
    m_process->waitForStarted(); // wait for process start
    writeCommand(ROOT_OF_FILE); // change root
    /* initiate cmd/terminal end */

    /* initiate widget*/
    // widget that can't be toggled before select stereoSound
    ui->pushButtonShowTargetInfo->setEnabled(false);
    ui->pushButtonSplit2mono->setEnabled(false);
    ui->pushButtonTransmit->setEnabled(false);

    /* connect */
    connect(m_process, &QProcess::readyReadStandardError, this, &Widget::show_processError);
    connect(m_process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(show_processError()));
}

Widget::~Widget()
{
    delete ui;
    m_process->close();
    m_process->waitForFinished();
    if (m_process)
    {
        delete m_process;
        m_process = nullptr;
    }
}

void Widget::writeCommand(QString cmd)
{
    cmd += "\r\n"; // indispensable
    QByteArray qbCmd = cmd.toLatin1();
    char * szCmd = qbCmd.data();
    ll nlen = cmd.length();
    m_process->write(szCmd, nlen);
    // display write command
}

// choose 1 stereo music file
void Widget::on_pushButtonChoose1stereo_clicked()
{
    QString qsCurrentPath = QDir::currentPath();
    qDebug() << "current path : " << qsCurrentPath << Qt::endl;
//    writeCommand(QString("cd %1").arg(QDir::toNativeSeparators(qsCurrentPath)));
    m_urlAudioFile = QFileDialog::getOpenFileUrl(this, "open Audio File", QUrl(), "WAV files(*.wav");
    if ( ! m_urlAudioFile.isEmpty() )
    {
        // print the urlLink of AudioFile
        qDebug() << "m_urlAudioFile : " << m_urlAudioFile << Qt::endl;
        ui->pushButtonShowTargetInfo->setEnabled(true);
        ui->pushButtonSplit2mono->setEnabled(true);
    }
}

void Widget::on_pushButtonSplit2mono_clicked()
{
    if ( m_urlAudioFile.isValid() == false)
    {
        qDebug() << " url File is invalid " << Qt::endl;
        return ;
    }
    QString qstrCmd = tr("ffmpeg -i %1 -filter_complex \"[0:a]channelsplit=channel_layout=stereo[left][right]\" -map \"[left]\" left.wav -map \"[right]\" right.wav\r\n"); // 末尾一定加上换行回车符，不然不会执行

}

void Widget::show_processError()
{
    QString qsErrorInfo = QString::fromLocal8Bit(m_process->readAllStandardError());
//    qDebug() << "Error code : " << m_process->exitCode();
    if (qsErrorInfo.length() <= 0) return;
    QMessageBox::critical(this, tr("process error"), qsErrorInfo + tr("\nerror code: %1").arg(m_process->exitCode()));
}

void Widget::on_pushButtonShowTargetInfo_clicked()
{
}

