#include "homePage.h"
#include "musicplayer.h"
#include "tcpclient.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  /* page switch */
  QApplication a(argc, argv);
  homePage w;
  tcpclient client;
  MusicPlayer player;
  QObject::connect(&w, &homePage::show_musicPlayer, &player, &MusicPlayer::ShowMyself);
  QObject::connect(&w, &homePage::show_tcpClient, &client, &tcpclient::ShowMyself);
  QObject::connect(&player, &MusicPlayer::show_tcpClient, &client, &tcpclient::ShowMyself);
  QObject::connect(&player, &MusicPlayer::show_mainWindow, &w, &homePage::ShowMyself);
  QObject::connect(&client, &tcpclient::show_mainWindow, &w, &homePage::ShowMyself);
  QObject::connect(&client, &tcpclient::show_musicPlayer, &player, &MusicPlayer::ShowMyself);

  /* music player */
  QObject::connect(&client, &tcpclient::evoke_music, &player, &MusicPlayer::ins_process);
  QObject::connect(&client, &tcpclient::evoke_scanLib, &player, &MusicPlayer::updatePlayList);

  /* home page */
  QObject::connect(&client, &tcpclient::evoke_homePage, &w, &homePage::ins_process);
  QObject::connect(&client, &tcpclient::evoke_setStatus, &w, &homePage::setStatus);
  QObject::connect(&player, &MusicPlayer::evoke_setChannel, &w, &homePage::setChannel);

  w.ShowMyself();
  return a.exec();
}
