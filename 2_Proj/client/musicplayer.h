#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "../public/codecodesys.h"
#include "../public/macro.h"
#include <QAudioOutput>
#include <QDebug>
#include <QDir>
#include <QMap>
#include <QDirIterator>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QList>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QThread>
#include <QThreadPool>
#include <QtDebug>

namespace Ui {
class MusicPlayer;
}

class MusicPlayer : public QMainWindow {
  Q_OBJECT

public:
  explicit MusicPlayer(QWidget *parent = nullptr);
  ~MusicPlayer() override;

  // action control
private:
    qint64 m_channelIndex;
    QMap<qint64, QString> m_toChannelName;

public:
  bool action_playBack(bool act); // act == true: play, act == false: pause
  bool action_Volume(qint64 percentage); // perc = [0, 100]
  bool action_rePlay();
  bool action_setSource(QString folderName);
  bool action_setChannel(qint64 channelIndex);

  // tool function
  void tool_scanLocalMusicFile();

private:
  Ui::MusicPlayer *ui;
  QList<QString> m_musicList; // list of ab path of music in Music foler of app home dir
  QMediaPlayer m_player;
  QAudioOutput m_audioOutput;

signals:
  void show_tcpClient();
  void show_mainWindow();

public slots:
  // instruction
  void ins_process(qint64 uAct_name, QString uAct_val); // prase and execute instructions

  // page button
  void ShowMyself(); // show page
  void on_pushButton_main_clicked();       // jump to main page
  void on_pushButton_tcpSetting_clicked(); // jump to tcpSettings page
  void on_pushButton_music_clicked();      // jump to music player page
private slots:

  // interact
  void on_btnPlay_clicked();

  };

#endif // MUSICPLAYER_H
