#ifndef RAWMEDIAPROCESS_H
#define RAWMEDIAPROCESS_H

#include <QMainWindow>
#include <QWidget>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QUrl>

#include "tcpController.h"
#include "processcontrol.h"
#include "../public/macro.h"

namespace Ui {
class rawMediaProcess;
}

class rawMediaProcess : public QMainWindow
{
    Q_OBJECT

public:
    explicit rawMediaProcess(QWidget *parent = nullptr);
    ~rawMediaProcess() override;
private:
    Ui::rawMediaProcess *ui;

    /* file */
private:
    QUrl m_urlAudioFile;
    QString m_workingPath = "D:/Dev/CourseDesign/MultiStereo/3_Resource/";
    QString m_rawMediaFolder = "MusicLibrary_raw/";
    QString m_mediaFolder = "MusicLibary/";

public slots:
    void on_pushButtonChoose1stereo_clicked(); // choose 1 stereo music file
    QString getFileInfo();


    /* processcontrol */
private slots:
    void on_pushButton_writeCmd_clicked(); // write commands to cmd console

    /* split into 2 channels */
    void on_pushButtonSplit2mono_clicked(); // split stereo file into seperate channel (split into several files)
public slots:
    bool split_2(const QString &srcFilePath); // split double track file into two individual file

signals:
    void evoke_music_synchronization(const QString &folderName, qint64 channelNumber); // transmit processed music files to client


private slots:
    void displayErrorInfo(QString msg); // display process std error infomation in textBrowser

    void displayOutPutInfo(QString msg); // display process std outPut  info in textBrowser

    /* split into 5 channels */
    void on_pushButtonGetFileInfo_clicked();

public slots:
    void on_pushButtonSplit6mono_clicked();
    bool split_6(const QString &srcFilePath);
};

#endif // RAWMEDIAPROCESS_H
