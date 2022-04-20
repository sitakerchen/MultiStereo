#ifndef TCPCONTROLLER_H
#define TCPCONTROLLER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QThread>
#include <QVector>
#include <QThreadPool>
#include <QtConcurrent>
#include <QFileInfo>
#include <QMessageBox>
#include <QRunnable>
#include <QDir>
#include <QFuture>
#include <QFileDialog>
#include "../public/mediafile.h"
#include "../public/macro.h"
#include "../public/codecodesys.h"
#include "identitycontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class tcpController; }
QT_END_NAMESPACE

class tcpController : public QMainWindow
{
    Q_OBJECT
public:
    explicit tcpController(QMainWindow *parent = nullptr);
    ~tcpController() override;
    void test();
private:
    Ui::tcpController *ui;
    QTcpServer *m_tcpServer; // tcp服务器 (myself)
    QList<QTcpSocket*> m_ListTcpClient; // 客户端列表
    QTcpSocket *m_currentClient; // 现在连接的客户端
    bool m_bIsListening; // 是否处于监听开启状态

public slots:
    void NewConnectionSlot(); // 处理新的连接请求
    void disConnectedSlot(); // 处理客户端的断开连接请求
    void on_pushButtonDisconnect_clicked(); // 开启与关闭服务器的监听与连接

    /* send and recv data*/
public slots:
    void sendFile(); // send media file to client
    void synchronize_musicFile(const QString &folderName, qint64 channelNumber); // synchronize local music library to client
    void ReadData(); // 读取来自客户端的数据
    qint64 sendData2single(QTcpSocket *client, const QByteArray &data); // send data to one single client
    qint64 sendData2all(const QByteArray &data); // send to all connected client

public slots:
    void on_pushButtonSend_clicked(); // 发送消息到客户端
    void on_pushButton_sendFile_clicked();

    /* remote synchronous music player controller */
public slots:
    void on_btnPlay_clicked();
    void on_ListWidget_musicName_doubleClicked(const QModelIndex &index);

    /* media file manage */
private:
    mediaFile m_outFile;
    QVector<QDir> m_musicDir; // dir that contains music
    QList<QFileInfo> m_musicInfo; // file info list of all accessible music
    QString m_MusicLibBasePath = "D:/Dev/CourseDesign/MultiStereo/3_Resource/MusicLibary/"; // base bath of music library

public slots:
    void on_pushButton_chooseFile_clicked(); // choose a file to be send
    void on_pushButton_PlayList_clicked(); // refresh the list widget

signals:
    void evoke_split(const QString &srcFilePath);

    /* UI */
public slots:
    void on_pushButtonClearWindow_clicked(); // 清除接收窗口内容
};

#endif // TCPCONTROLLER_H
