#include "mainwindow.h"
#include "musicplayer.h"
#include "tcpclient.h"

#include <QApplication>

int main(int argc, char *argv[]) {
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

  w.show();
  return a.exec();
}
