#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QObject>
#include <QDirIterator>
#include <QtDebug>
#include <QString>
#include <QList>
#include <QFileInfo>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>
#include <QMutex>
#include <QThreadPool>

namespace Ui {
class MusicPlayer;
}

class MusicPlayer : public QMainWindow {
  Q_OBJECT

public:
  explicit MusicPlayer(QWidget *parent = nullptr);
  ~MusicPlayer();

private:
  Ui::MusicPlayer *ui;


signals:
    void show_tcpClient();
    void show_mainWindow();

public slots:
  void ShowMyself(); // show page

  // page button
  void on_pushButton_main_clicked();       // jump to main page
  void on_pushButton_tcpSetting_clicked(); // jump to tcpSettings page
  void on_pushButton_music_clicked();      // jump to music player page
};

#endif // MUSICPLAYER_H
