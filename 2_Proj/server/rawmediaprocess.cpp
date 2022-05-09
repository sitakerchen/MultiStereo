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

    /* to music lib */
    ProcessControl::getInstance().setWorkingPath(m_workingPath + m_rawMediaFolder);
}

rawMediaProcess::~rawMediaProcess()
{
    delete ui;
}

void rawMediaProcess::delay_ms(qint64 ms)
{
    calculator &cal = calculator::getInstance();
    ms = cal.playDelay_server(cal.getDelayTime());
    QTime dieTime = QTime::currentTime().addMSecs(ms);
         while( QTime::currentTime() < dieTime )
             QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    qDebug() << "server delay = " << ms << endl;
}

void rawMediaProcess::on_pushButtonChoose1stereo_clicked()
{
    PC::getInstance().setWorkingPath(m_workingPath + m_rawMediaFolder);
    m_urlAudioFile = QFileDialog::getOpenFileUrl(this, "open Audio File", QUrl(), "WAV MP3 files(*.mp3 *.wav)");
    if ( ! m_urlAudioFile.isEmpty() )
    {
        qDebug() << "m_urlAudioFile path: " << m_urlAudioFile.path() << Qt::endl; // print the url-Link-Path of AudioFile
        ui->pushButtonSplit2mono->setEnabled(true);
    }
}

qint64 rawMediaProcess::getFileChannels(const QString &srcFilePath, qint64 &nChannels)
{
    QUrl scrFile(srcFilePath);
    m_urlAudioFile = scrFile;
    if ( m_urlAudioFile.isValid() == false )
    {
        qDebug() << " url File is invalid " << Qt::endl;
        nChannels = -1;
        return -1;
    }
    on_pushButtonGetFileInfo_clicked();
    PC::getInstance().wait_forFinished();
    qDebug() << "file : " << m_urlAudioFile.fileName() << "\ninfo" << m_ffmpegFileInfo << endl;
    QString qstrAim = "channels=(\\d)";
    QRegExp rx(qstrAim);
    qint64 nPos = rx.indexIn(m_ffmpegFileInfo);
    if (nPos >= 0)
    {
        qint64 nNumber = rx.cap(1).toInt();
        qDebug() << "channels = " << nNumber;
        nChannels = nNumber;
        return nNumber;
    }
    nChannels = -1;
    return -1;
}

void rawMediaProcess::on_pushButtonSplit2mono_clicked()
{
    if ( m_urlAudioFile.isValid() == false)
    {
        qDebug() << " url File is invalid " << Qt::endl;
        return;
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
            QString qstrOldFile = QString(path).replace("%1", m_rawMediaFolder);
            QString qstrNewFile = QString(path).replace("%1", m_mediaFolder + m_urlAudioFile.fileName().section('.', 0, 0) + "/");
            qDebug() << "old file path = " << qstrOldFile << endl;
            qDebug() << "new file path = " << qstrNewFile << endl;
            bool ok = dir.rename(qstrOldFile, qstrNewFile);
            if (ok == false)
            {
                QMessageBox::critical(this, __FUNCTION__, tr("split fail!\n oldFile:%1 \n newFile:%2").arg(qstrOldFile, qstrNewFile));
                return;
            }
        }
    }
    dir.cd(m_urlAudioFile.fileName().section('.', 0, 0));
    qDebug() << "cur dir : " <<  dir << endl;
    dir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
    qint64 nChannelNumber = dir.count();
    emit evoke_music_synchronization(m_urlAudioFile.fileName().section('.', 0, 0), nChannelNumber); // 有几个文件就算几个声道
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
            QString qstrOldFile = QString(path).replace("%1", m_rawMediaFolder);
            QString qstrNewFile = QString(path).replace("%1", m_mediaFolder + m_urlAudioFile.fileName().section('.', 0, 0) + "/");
            qDebug() << "old file path = " << qstrOldFile << endl;
            qDebug() << "new file path = " << qstrNewFile << endl;
            bool ok = dir.rename(qstrOldFile, qstrNewFile);
            if (ok == false)
            {
                QMessageBox::critical(this, __FUNCTION__, tr("split fail!\n oldFile:%1 \n newFile:%2").arg(qstrOldFile, qstrNewFile));
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

void rawMediaProcess::on_pushButtonSplit6mono_clicked()
{
    if ( m_urlAudioFile.isValid() == false)
    {
        qDebug() << " url File is invalid " << Qt::endl;
        return ;
    }
    QString qstrCmd = FFMPEG_SPLIT_5.arg( m_urlAudioFile.fileName(), m_urlAudioFile.fileName().section('.', 0, 0) );
    qDebug() << "qstrCmd" << qstrCmd << endl;
    ProcessControl::getInstance().setWorkingPath(m_workingPath + m_rawMediaFolder);
    QDir dir(m_workingPath + m_mediaFolder);
    if (!dir.exists(m_urlAudioFile.fileName().section('.', 0, 0)))
    {
        PC::getInstance().writeCommand(qstrCmd);
        dir.mkdir(m_urlAudioFile.fileName().section('.', 0, 0));
        QString FilePathList[6] =
        {
            (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_5_LEFT_FORE+ ".wav"),
            (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_5_LEFT+ ".wav"),
            (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_5_LEFT_BACK+ ".wav"),
            (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_5_RIGHT_FORE+ ".wav"),
            (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_5_RIGHT + ".wav"),
            (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_5_RIGHT_BACK + ".wav"),
        };
        ProcessControl::getInstance().wait_forFinished();
        for (auto path: FilePathList)
        {
            QString qstrOldFile = QString(path).replace("%1", m_rawMediaFolder);
            QString qstrNewFile = QString(path).replace("%1", m_mediaFolder + m_urlAudioFile.fileName().section('.', 0, 0) + "/");
            qDebug() << "old file path = " << qstrOldFile << endl;
            qDebug() << "new file path = " << qstrNewFile << endl;
            bool ok = dir.rename(qstrOldFile, qstrNewFile);
            if (ok == false)
            {
                QMessageBox::critical(this, __FUNCTION__, tr("split fail!\n oldFile:%1 \n newFile:%2").arg(qstrOldFile, qstrNewFile));
                return;
            }
        }
    }
    dir.cd(m_urlAudioFile.fileName().section('.', 0, 0));
    qDebug() << "cur dir : " <<  dir << endl;
}

bool rawMediaProcess::split_6(const QString &srcFilePath)
{
    QUrl srcFile(srcFilePath);
    m_urlAudioFile = srcFile;
    if ( m_urlAudioFile.isValid() == false)
    {
        qDebug() << " url File is invalid " << Qt::endl;
        return false;
    }
    QString qstrCmd = FFMPEG_SPLIT_5.arg( m_urlAudioFile.fileName(), m_urlAudioFile.fileName().section('.', 0, 0) );
    qDebug() << "qstrCmd" << qstrCmd << endl;
    QDir dir(m_workingPath + m_mediaFolder);
    if (!dir.exists(m_urlAudioFile.fileName().section('.', 0, 0)))
    {
        PC::getInstance().writeCommand(qstrCmd);
        dir.mkdir(m_urlAudioFile.fileName().section('.', 0, 0));
        QString FilePathList[6] =
        {
            (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_5_LEFT_FORE+ ".wav"),
            (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_5_LEFT+ ".wav"),
            (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_5_LEFT_BACK+ ".wav"),
            (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_5_RIGHT_FORE+ ".wav"),
            (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_5_RIGHT + ".wav"),
            (m_workingPath + "%1" + m_urlAudioFile.fileName().section('.', 0, 0) + "_" + FILE_CHANNEL_NAME_5_RIGHT_BACK + ".wav"),
        };
        ProcessControl::getInstance().wait_forFinished();
        for (auto path: FilePathList)
        {
            QString qstrOldFile = QString(path).replace("%1", m_rawMediaFolder);
            QString qstrNewFile = QString(path).replace("%1", m_mediaFolder + m_urlAudioFile.fileName().section('.', 0, 0) + "/");
            qDebug() << "old file path = " << qstrOldFile << endl;
            qDebug() << "new file path = " << qstrNewFile << endl;
            bool ok = dir.rename(qstrOldFile, qstrNewFile);
            if (ok == false)
            {
                QMessageBox::critical(this, __FUNCTION__, tr("split fail!\n oldFile:%1 \n newFile:%2").arg(qstrOldFile, qstrNewFile));
                return false;
            }
        }
    }
    dir.cd(m_urlAudioFile.fileName().section('.', 0, 0));
    qDebug() << "cur dir : " <<  dir << endl;
    return true;
}

void rawMediaProcess::displayErrorInfo(QString msg)
{
    ui->textBrowserError->append(msg);
}

void rawMediaProcess::displayOutPutInfo(QString msg)
{
    m_ffmpegFileInfo += msg;
    ui->textBrowserOutPut->append(msg);
}

void rawMediaProcess::on_pushButton_writeCmd_clicked()
{
    QString qstrCmd = ui->lineEdit_cmdInput->text();
    PC::getInstance().writeCommand(qstrCmd);
}

void rawMediaProcess::to_workingPath()
{
    ProcessControl::getInstance().setWorkingPath(m_workingPath + m_rawMediaFolder);
}

void rawMediaProcess::on_pushButtonGetFileInfo_clicked()
{
     if ( m_urlAudioFile.isValid() == false)
    {
        qDebug() << " url File is invalid " << Qt::endl;
        return;
    }
    QString qstrCmd = FFMPEG_GET_MEDIA_INFO.arg( m_urlAudioFile.fileName());
//    ProcessControl::getInstance().setWorkingPath(m_workingPath + m_rawMediaFolder);
    m_ffmpegFileInfo = "";
    PC::getInstance().writeCommand(qstrCmd);
}

