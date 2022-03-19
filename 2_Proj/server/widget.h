#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QUrl>

#include "tcpserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget() override;

private slots:
    void on_pushButtonChoose1stereo_clicked();
    void on_pushButtonSplit2mono_clicked();
    void on_pushButtonTransmit_clicked();
    void displayErrorInfo(QString msg);
    void displayOutPutInfo(QString msg);
//    void on_pushButtonShowTargetInfo_clicked();

private:
    Ui::Widget *ui;
    QUrl m_urlAudioFile;
    tcpserver *m_ui_server;

};
#endif // WIDGET_H