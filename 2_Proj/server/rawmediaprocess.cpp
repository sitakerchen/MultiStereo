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
    QString qstrCmd = FFMPEG_SPLIT_2.arg( m_urlAudioFile.fileName(), m_urlAudioFile.fileName().section('.', 0, 0) );
    qDebug() << "qstrCmd" << qstrCmd << endl;
    ProcessControl::getInstance().setWorkingPath(m_workingPath + m_rawMediaFolder);
    QDir dir(m_workingPath + m_mediaFolder);
     if (dir.exists(m_urlAudioFile.fileName().section('.', 0, 0)))
    {
        qDebug() << "already exist" << endl;
        return ;
    }
    PC::getInstance().writeCommand(qstrCmd);
    dir.mkdir(m_urlAudioFile.fileName().section('.', 0, 0));
    QString FilePathList[2] =
    {
        (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_2_LEFT + ".wav"),
        (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_2_RIGHT + ".wav")
    };
    ProcessControl::getInstance().wait_forFinished();
    for (auto path: FilePathList)
    {
        QString qstrOldFile = path.arg(m_rawMediaFolder), qstrNewFile = path.arg(m_mediaFolder + m_urlAudioFile.fileName().section('.', 0, 0) + "/");
        bool ok = dir.rename(qstrOldFile, qstrNewFile);
        if (ok == false)
        {
            QMessageBox::critical(this, __FUNCTION__, tr("split fail!"));
            return ;
        }
    }

}

bool rawMediaProcess::split_2(const QString &srcFilePath)
{
    QUrl scrFile(srcFilePath);
    m_urlAudioFile = scrFile;
    if ( m_urlAudioFile.isValid() == false)
    {
        qDebug() << " url File is invalid " << Qt::endl;
        return false;
    }
    QString qstrCmd = FFMPEG_SPLIT_2.arg( m_urlAudioFile.fileName(), m_urlAudioFile.fileName().section('.', 0, 0) );
    qDebug() << "qstrCmd" << qstrCmd << endl;
    ProcessControl::getInstance().setWorkingPath(m_workingPath + m_rawMediaFolder);
    QDir dir(m_workingPath + m_mediaFolder);
    if (!dir.exists(m_urlAudioFile.fileName().section('.', 0, 0)))
    {
        PC::getInstance().writeCommand(qstrCmd);
        dir.mkdir(m_urlAudioFile.fileName().section('.', 0, 0));
        QString FilePathList[2] =
        {
            (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_2_LEFT + ".wav"),
            (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_2_RIGHT + ".wav")
        };
        ProcessControl::getInstance().wait_forFinished();
        for (auto path: FilePathList)
        {
            QString qstrOldFile = path.arg(m_rawMediaFolder), qstrNewFile = path.arg(m_mediaFolder + m_urlAudioFile.fileName().section('.', 0, 0) + "/");
            bool ok = dir.rename(qstrOldFile, qstrNewFile);
            if (ok == false)
            {
                QMessageBox::critical(this, __FUNCTION__, tr("split fail!"));
                return false;
            }
        }
    }
    dir.cd(m_urlAudioFile.fileName().section('.', 0, 0));
    qDebug() << "cur dir : " <<  dir << endl;
    dir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
    qint64 nChannelNumber = dir.count();
    emit evoke_music_synchronization(m_urlAudioFile.fileName().section('.', 0, 0), nChannelNumber); // 有几个文件就算几个声道
    return true;
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

