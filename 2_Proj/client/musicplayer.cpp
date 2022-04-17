#include "musicplayer.h"
#include "ui_musicplayer.h"
#include "../public/macro.h"
using Qt::endl;

MusicPlayer::MusicPlayer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MusicPlayer) {
  ui->setupUi(this);
  // init player
  m_player.setAudioOutput(&m_audioOutput);
//  connect(&m_player, SIGNAL(positionChanged(qint64)), this, ) // wait for ui
  m_musicList.append(PATH_ANDROID_APP_MUSIC+ "Left_RightStereoSoundTest-320k.mp3");
  qDebug() << "test file path :" << m_musicList.at(0) << endl;
  QDir dir(PATH_ANDROID_APP_MUSIC);
  if (dir.exists(m_musicList.at(0)))
    m_player.setSource(QUrl::fromLocalFile(m_musicList.at(0)));
  else
    QMessageBox::warning(this, tr("error"), tr("file not exist"));
}

MusicPlayer::~MusicPlayer() { delete ui; }

bool MusicPlayer::action_playBack(bool act)
{
    if (act) m_player.play();
    else m_player.pause();
    return true;
}

bool MusicPlayer::action_Volume(qint64 per)
{
    m_audioOutput.setVolume(per);
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

void MusicPlayer::ins_process(QString ins)
{
    qint64 op;
    qint64 temp = ins.section("##", 3, 3).toInt();
    action_playBack(temp);
}

