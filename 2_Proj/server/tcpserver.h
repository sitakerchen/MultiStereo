#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QFileDialog>
#include "../public/mediafile.h"

QT_BEGIN_NAMESPACE
namespace Ui { class tcpserver; }
QT_END_NAMESPACE

class tcpserver : public QMainWindow
{
    Q_OBJECT
public:
    explicit tcpserver(QMainWindow *parent = nullptr);
    ~tcpserver();

private:
    Ui::tcpserver *ui;
    QTcpServer *m_tcpServer; // tcp服务器
    QList<QTcpSocket*> m_ListTcpClient; // 客户端列表
    QTcpSocket *m_currentClient; // 现在连接的客户端
    mediaFile m_outFile;
    bool m_bIsListening; // 是否处于监听开启状态

private:
    void sendData();

private slots:
    // server slot function
    void ReadData(); // 读取来自客户端的数据
    void NewConnectionSlot(); // 处理新的连接请求
    void disConnectedSlot(); // 处理客户端的断开连接请求

    // button
    void on_pushButtonDisconnect_clicked(); // 开启与关闭服务器的监听与连接
    void on_pushButtonClearWindow_clicked(); // 清除接收窗口内容
    void on_pushButtonSend_clicked(); // 发送数据到客户端
    void on_pushButton_chooseFile_clicked();
    void on_pushButton_sendFile_clicked();
};

#endif // TCPSERVER_H
