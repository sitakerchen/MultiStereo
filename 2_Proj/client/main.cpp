#include "mainwindow.h"
#include "musicplayer.h"
#include "tcpclient.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    // page switch
  QApplication a(argc, argv);
  MainWindow w;
  tcpclient client;
  MusicPlayer player;
  QObject::connect(&w, &MainWindow::show_musicPlayer, &player, &MusicPlayer::ShowMyself);
  QObject::connect(&w, &MainWindow::show_tcpClient, &client, &tcpclient::ShowMyself);
  QObject::connect(&player, &MusicPlayer::show_tcpClient, &client, &tcpclient::ShowMyself);
  QObject::connect(&player, &MusicPlayer::show_mainWindow, &w, &MainWindow::ShowMyself);
  QObject::connect(&client, &tcpclient::show_mainWindow, &w, &MainWindow::ShowMyself);
  QObject::connect(&client, &tcpclient::show_musicPlayer, &player, &MusicPlayer::ShowMyself);

  // music player
  QObject::connect(&client, &tcpclient::evoke_music, &player, &MusicPlayer::ins_process);

  w.show();
  return a.exec();
}
