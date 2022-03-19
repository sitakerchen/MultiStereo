#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class tcpclient; }
QT_END_NAMESPACE

class tcpclient : public QMainWindow
{
    Q_OBJECT

public:
    explicit tcpclient(QWidget *parent = nullptr);
    ~tcpclient();

private:
    Ui::tcpclient *ui;
    QTcpSocket *tcpClient; // 客户端
    bool isConnected;

private slots:
    // client slot function
    void ReadData(); // 读取服务器发送过来的数据
    void ReadError(QAbstractSocket::SocketError); // 连接出错处理
    // button
    void on_pushButtonDisconnect_clicked(); // 断开与服务器的连接
    void on_pushButtonClearWindow_clicked(); // 清除接收窗口
    void on_pushButtonSend_clicked(); // 发送数据到服务器
};
#endif // MAINWINDOW_H
