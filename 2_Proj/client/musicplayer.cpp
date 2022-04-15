#include "musicplayer.h"
#include "ui_musicplayer.h"
#include "macro.h"

MusicPlayer::MusicPlayer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MusicPlayer) {
  ui->setupUi(this);
}

MusicPlayer::~MusicPlayer() { delete ui; }



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
