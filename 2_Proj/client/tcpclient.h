#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "../public/mediafile.h"
#include "../public/macro.h"
#include "../public/codecodesys.h"
#include <QHostAddress>
#include <QMainWindow>
#include <QDir>
#include <QMessageBox>
#include <QTcpSocket>
#include <QScroller>

QT_BEGIN_NAMESPACE
namespace Ui {
class tcpclient;
}
QT_END_NAMESPACE

class tcpclient : public QMainWindow {
  Q_OBJECT

public:
  explicit tcpclient(QWidget *parent = nullptr);
  ~tcpclient();
  void create_homeDir(); // save media file
  void RecvFile(); // recv media file from server
  void Reset_fileRecvStatus(); // reset kinds of variables of mediaFile
  qint64 get_INS_length(); // return the length of a INS

private:
  Ui::tcpclient *ui;
  QTcpSocket *m_tcpClient; // 客户端
  mediaFile m_mdiFile;     // media file and its relevant properties
  bool m_isConnected;      // 是否处于已连接状态
  bool m_nIsINS;          // 是否为文件头部分

signals:
    void show_mainWindow();
    void show_musicPlayer();
    void evoke_music(QString ins);

public slots:
  // client slot function
  void ReadData(); // 读取服务器发送过来的数据
  void ReadError(QAbstractSocket::SocketError); // 连接出错处理
  void SendData(QString msg);                   // 发送数据到服务器

  // function button
  void on_pushButtonDisconnect_clicked();  // 断开与服务器的连接
  void on_pushButtonClearWindow_clicked(); // 清除接收窗口

  // page button
  void ShowMyself(); // show page

  void on_pushButton_main_clicked();       // jump to main page
  void on_pushButton_tcpSetting_clicked(); // jump to tcpSettings page
  void on_pushButton_music_clicked();      // jump to music player page
};
#endif // MAINWINDOW_H
