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

    /* processcontrol */
private slots:
    void on_pushButton_writeCmd_clicked(); // write commands to cmd console

public:
//    void on_pushButtonShowTargetInfo_clicked(); // show info of chosen stereo file (some bugs, ignore this function temporary)

    /* split into 2 channels */

public slots:
    void on_pushButtonChoose1stereo_clicked(); // choose 1 stereo music file

    void on_pushButtonSplit2mono_clicked(); // split stereo file into seperate channel (split into several files)

public slots:
    void displayErrorInfo(QString msg); // display process std error infomation in textBrowser

    void displayOutPutInfo(QString msg); // display process std outPut  info in textBrowser

    /* split into 5 channels */};

#endif // RAWMEDIAPROCESS_H
