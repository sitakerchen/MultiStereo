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

  /* init widget */
  ui->listWidget_musicList->setWordWrap(true);

  /* connect */
  connect(&m_player, &QMediaPlayer::errorOccurred, [](QMediaPlayer::Error error, const QString &errorStr){
      qDebug() << "player error! " << error << " " << errorStr << endl;
  });
  connect(&m_player, &QMediaPlayer::positionChanged, this, &MusicPlayer::updatePosition);
  connect(&m_player, &QMediaPlayer::durationChanged, this, &MusicPlayer::updateDuration);
  connect(&m_player, &QMediaPlayer::mediaStatusChanged, this ,[=](QMediaPlayer::MediaStatus status){
      qDebug() << "mediaStatusChanged " << status << endl;
  });
  connect(&m_player, &QMediaPlayer::sourceChanged, this, [=](const QUrl &media){
     qDebug() <<"mediaSourceChanged source: " << media << endl;
  });
}

MusicPlayer::~MusicPlayer()
{
    for (auto fileList: m_musicMap)
    {
        if (fileList)
        {
            delete fileList;
            fileList = nullptr;
        }
    }
    delete ui;
}

void MusicPlayer::action_playBack(bool act)
{
    qDebug() << __FUNCTION__ << endl;
    if (act) m_player.play();
    else m_player.pause();
    updatePlayBtnIcon();
}

void MusicPlayer::action_Volume(qint64 per)
{
    qDebug() << __FUNCTION__ << endl;
    m_audioOutput.setVolume(static_cast<double>(per) / 100.0);
}

void MusicPlayer::action_rePlay()
{
    qDebug() << __FUNCTION__ << endl;
    m_player.stop();
    m_player.play();
    updatePlayBtnIcon();
}

void MusicPlayer::action_setSource(QString folderName)
{
    qDebug() << __FUNCTION__ << endl;
    if (m_musicMap.find(folderName) == m_musicMap.end())
    {
        qDebug() << __FUNCTION__ << "musicFile not found" << endl;
        QMessageBox::critical(this, __FUNCTION__, "musicFile not found");
        return;
    }
    QString qstrMusic = folderName + "_" + m_toChannelName[m_channelIndex] + ".wav";
    for (auto music: *m_musicMap[folderName])
    {
        if (music.fileName() == qstrMusic)
        {
            qDebug() << "music path = " << music.absoluteFilePath() << endl;
            m_player.setSource(QUrl::fromLocalFile(music.absoluteFilePath()));

            return;
        }
    }
    qDebug() << "use default music file" << endl;
    auto defaultMusic = m_musicMap[folderName]->begin()->absoluteFilePath();
    m_player.setSource(QUrl::fromLocalFile(defaultMusic));
}

void MusicPlayer::action_setChannel(qint64 channelIndex)
{
    qDebug() << __FUNCTION__ << endl;
    m_channelIndex = channelIndex;
}

void MusicPlayer::action_setPos(qint64 pos)
{
    qDebug() << __FUNCTION__ << endl;
    QMediaPlayer::PlaybackState before = m_player.playbackState();
    m_player.pause();
    m_player.setPosition(pos);
    if (before == QMediaPlayer::PlayingState) m_player.play();
}

QString MusicPlayer::tool_forMatTime(qint64 timeMilliSeconds)
{
    qint64 seconds = timeMilliSeconds / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}

void MusicPlayer::updatePosition(qint64 pos)
{
    ui->positionSlider->setValue(static_cast<int>(pos));
    ui->Label_position->setText(tool_forMatTime(pos));
    ui->Label_position_2->setText(tool_forMatTime(m_player.duration()));
}

void MusicPlayer::updateDuration(qint64 duration)
{
    ui->positionSlider->setRange(0, static_cast<int>(duration));
    ui->positionSlider->setEnabled(static_cast<int>(duration) > 0);
    ui->positionSlider->setPageStep(static_cast<int>(duration));
    ui->Label_position_2->setText(tool_forMatTime(m_player.duration()));
}

void MusicPlayer::updatePlayBtnIcon()
{
    if (m_player.playbackState() == QMediaPlayer::PausedState)
    {
        ui->btnPlay->setIcon(QIcon(":/image/image/image/24gl-play.png"));
    }
    else
    {
        ui->btnPlay->setIcon(QIcon(":/image/image/image/24gl-pause.png"));
    }
}

void MusicPlayer::updatePlayList()
{
    ui->listWidget_musicList->clear();
    QDirIterator dir_iterator(PATH_ANDROID_APP_MUSIC, QDir::NoDotAndDotDot | QDir::AllDirs); // scan all the dirs
    while (dir_iterator.hasNext())
    {
        dir_iterator.next();
        QString qstrFileFolder = dir_iterator.fileName();
        qDebug() << "file name = " << qstrFileFolder << endl;
        if (m_musicMap.find(qstrFileFolder) == m_musicMap.end()) m_musicMap[qstrFileFolder] = new QFileInfoList;
        m_musicMap[qstrFileFolder]->clear();

        /* add to file info map */
        ui->listWidget_musicList->addItem(qstrFileFolder); // add file name to playList
        qDebug() << "folder path = " << dir_iterator.filePath() << endl;
        QDirIterator dir_iterator_file(dir_iterator.filePath(), QDir::NoDotAndDotDot | QDir::Files);
        while (dir_iterator_file.hasNext())
        {
            dir_iterator_file.next();
            qDebug() << "file path = " << dir_iterator_file.filePath() << endl;
            m_musicMap[qstrFileFolder]->append(dir_iterator_file.fileInfo());
        }
    }
    ui->listWidget_musicList->sortItems();
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

void MusicPlayer::ins_process(qint64 uAct_name, QString uAct_val)
{
    qDebug() << "in music";
    qDebug() << "uAct_name = " << uAct_name <<  "uAct_val = " << uAct_val << endl;
//    ui->listWidget_musicList->addItem(m_player.source().toString());
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
    case ACT_NAME_SET_VOLUME:
        action_Volume(uAct_val.toInt());
        break;
    case ACT_NAME_SET_POS:
        action_setPos(uAct_val.toInt());
        break;
//    case :
//       break;
//    case :
//       break;
//    case :
//       break;
//    case :
//        break;

    default:
        qDebug() << "uAct_name: " << uAct_name << " no valid" << endl;
        break;
    }
}

