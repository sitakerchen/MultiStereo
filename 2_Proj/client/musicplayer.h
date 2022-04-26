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

private:
    Ui::MusicPlayer *ui;
    QMap<QString, QFileInfoList*> m_musicMap; // list of ab path of music in Music foler of app home dir
    QMediaPlayer m_player;
    QAudioOutput m_audioOutput;

  // action
private:
    qint64 m_channelIndex;
    QMap<qint64, QString> m_toChannelName;

public:
    void action_playBack(bool act); // act == true: play, act == false: pause
    void action_Volume(qint64 percentage); // perc = [0, 100]
    void action_rePlay();
    void action_setSource(QString folderName);
    void action_setChannel(qint64 channelIndex);
    void action_setPos(qint64 pos);

    void tool_scanLocalMusicFile();
    QString tool_forMatTime(qint64 timeMilliSeconds);

public slots:
    void updatePosition(qint64 pos);
    void updateDuration(qint64 duration);
    void updatePlayBtnIcon(); // no need
    void updatePlayList();


     // instruction
public slots:
    void ins_process(qint64 uAct_name, QString uAct_val); // prase and execute instructions

    // page button
signals:
    void show_tcpClient();
    void show_mainWindow();

public slots:
    void ShowMyself(); // show page
    void on_pushButton_main_clicked();       // jump to main page
    void on_pushButton_tcpSetting_clicked(); // jump to tcpSettings page
    void on_pushButton_music_clicked();      // jump to music player page
};

#endif // MUSICPLAYER_H
