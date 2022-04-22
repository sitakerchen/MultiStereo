#include "musicplayer.h"
#include "ui_musicplayer.h"
#include "../public/macro.h"
using Qt::endl;

MusicPlayer::MusicPlayer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MusicPlayer) {
  ui->setupUi(this);
  // init player
  m_player.setAudioOutput(&m_audioOutput);

  /* init variables */
  m_channelIndex = 1; // default
  m_toChannelName[FILE_CHANNEL_2_LEFT] = FILE_CHANNEL_NAME_2_LEFT;
  m_toChannelName[FILE_CHANNEL_2_RIGHT] = FILE_CHANNEL_NAME_2_RIGHT;
  m_toChannelName[FILE_CHANNEL_5_LEFT_FORE] = FILE_CHANNEL_NAME_5_LEFT_FORE;
  m_toChannelName[FILE_CHANNEL_5_LEFT] =  FILE_CHANNEL_NAME_5_LEFT;
  m_toChannelName[FILE_CHANNEL_5_LEFT_BACK] = FILE_CHANNEL_NAME_5_LEFT_BACK;
  m_toChannelName[FILE_CHANNEL_5_RIGHT_FORE] = FILE_CHANNEL_NAME_5_RIGHT_FORE;
  m_toChannelName[FILE_CHANNEL_5_RIGHT] = FILE_CHANNEL_NAME_5_RIGHT;
  m_toChannelName[FILE_CHANNEL_5_RIGHT_BACK] = FILE_CHANNEL_NAME_5_RIGHT_BACK;

  /* connect */
  connect(&m_player, &QMediaPlayer::errorOccurred, [](QMediaPlayer::Error error, const QString &errorStr){
      qDebug() << "player error! " << error << " " << errorStr << endl;
  });
}

MusicPlayer::~MusicPlayer() { delete ui; }

bool MusicPlayer::action_playBack(bool act)
{
    qDebug() << "has audio? :" << m_player.hasAudio() << endl;
    if (act) m_player.play();
    else m_player.pause();
    return true;
}

bool MusicPlayer::action_Volume(qint64 per)
{
    m_audioOutput.setVolume(per);
    return true;
}

bool MusicPlayer::action_rePlay()
{
    m_player.stop();
    m_player.play();
    return true;
}

bool MusicPlayer::action_setSource(QString folderName)
{
    QDir dir(PATH_ANDROID_APP_MUSIC);
    if (!dir.exists(folderName))
    {
        qDebug() << tr("error, music file folder [%1] not exist").arg(folderName) << endl;
        return false;
    }
    dir.cd(folderName);
    QString qstrSourceName = folderName + "_" + m_toChannelName[m_channelIndex] + ".wav";
    qDebug() << "Source Name = " << qstrSourceName << endl;

    if (dir.exists(qstrSourceName))
    {
        qDebug() << "Source Name = " << qstrSourceName << endl;
        qDebug() << "Source path = " << dir.absoluteFilePath(qstrSourceName);
        m_player.setSource(QUrl::fromLocalFile(dir.absoluteFilePath(qstrSourceName)));
        return true;
    }
    else
    {
        QStringList filters = {"*.mp3", "*.wav"};
        QDirIterator dir_iterator(dir.absolutePath(), filters, QDir::NoDotAndDotDot);
        while (dir_iterator.hasNext())
        {
            dir_iterator.next();
            m_player.setSource(QUrl::fromLocalFile(dir_iterator.filePath()));
            qDebug() << tr("error, music file [%1] not exist, playing the first music file by default").arg(qstrSourceName) << endl;
            return false;
        }
        QMessageBox::critical(this, tr(__FUNCTION__), tr("empty folder!"));
        return false;
    }
}

bool MusicPlayer::action_setChannel(qint64 channelIndex)
{
    if (channelIndex < 1 or channelIndex > 8)
    {
        qDebug() << "error, invalid channel index " << endl;
        return false;
    }
    m_channelIndex = channelIndex;
    return true;
}



void MusicPlayer::ShowMyself()
{
  show();
}

void MusicPlayer::on_pushButton_main_clicked() {
  hide();
  emit show_mainWindow();
}

void MusicPlayer::on_pushButton_tcpSetting_clicked() {
  hide();
  emit show_tcpClient();
}

void MusicPlayer::on_pushButton_music_clicked() {
    ShowMyself();
}

void MusicPlayer::on_btnPlay_clicked()
{
    if (m_player.playbackState() == QMediaPlayer::PlayingState)
    {
        action_playBack(false);
        qDebug() << "status : false" << endl;
    }
    else
    {
        action_playBack(true);
        qDebug() << "status : true" << endl;
    }
}

void MusicPlayer::ins_process(qint64 uAct_name, QString uAct_val)
{
    qDebug() << "in music" << endl;
    qDebug() << "uAct_name = " << uAct_name <<  "uAct_val = " << uAct_val << endl;
    qDebug() << "player's source = " << m_player.source() << endl;
    switch (uAct_name) {
    case ACT_NAME_PLAYBACK:
        action_playBack(uAct_val.toInt());
        break;
    case ACT_NAME_REPLAY:
        action_rePlay();
        break;
    case ACT_NAME_SET_SOURCE:
        action_setSource(uAct_val);
        break;
    case ACT_NAME_ASSIGN_CHANNEL:
        action_setChannel(uAct_val.toInt());
        break;
    default:
        break;
    }
}

