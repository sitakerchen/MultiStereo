#ifndef TCPCONTROLLER_H
#define TCPCONTROLLER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QNetworkInterface>
#include <QThread>
#include <QVector>
#include <QThreadPool>
#include <QtConcurrent>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>
#include <QFuture>
#include <QFileDialog>
#include "../public/mediafile.h"
#include "../public/macro.h"
#include "../public/delayCalculator.h"
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
signals:
    void evoke_homePage_addItem(qint64 id);
    void evoke_homePage_RemoveItem(qint64 id);

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
private:
    QMediaPlayer m_player;
    QAudioOutput m_audioOutput;
    QFileInfo m_curSelectSong;

public slots:
    void delay_ms(qint64 ms = 0);
    void updatePosition(qint64 pos);  // no need
    void updateDuration(qint64 duration); // no need to send which can do by itself
    void updatePlayBtnIcon(); // no need
    QString forMatTime(qint64 timeMilliSeconds); // no need

    void on_btnPlay_clicked(); //
    void on_btnPre_clicked(); // no need, = setSource + replay
    void on_btnNext_clicked(); // no need, = setSource + replay
    void on_volumeSlider_valueChanged(int value); // !! value = [0, 1] double
    void on_btnVolume_clicked(); // no need
    void on_pushButton_rePlay_clicked(); // replay a song
    void on_ListWidget_musicName_doubleClicked(const QModelIndex &index); // no need, do it in setPlayerSource
    void setChannel(qint64 id, qint64 channelNumber); //
    void setPosition(int pos); //
    bool setPlayerSource(); //
    bool setPlayerSource(QString abPath);  // no need
    qint64 assignId();

    /* media file manage */
private:
    mediaFile m_outFile;
    QVector<QDir> m_musicDir; // dir that contains music
    QList<QFileInfo> m_musicInfoList; // file info list of all accessible music
    QString m_MusicLibBasePath = "D:/Dev/CourseDesign/MultiStereo/3_Resource/MusicLibary/"; // base bath of music library

public slots:
    void on_pushButton_chooseFile_clicked(); // choose a file to be send
    void on_pushButton_PlayList_clicked(); // refresh the list widget

signals:
    void evoke_split2(const QString &srcFilePath);

    /* UI */
public slots:
    void on_pushButtonClearWindow_clicked(); // 清除接收窗口内容


};

#endif // TCPCONTROLLER_H
