#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget() override;

public:
    void writeCommand(QString cmd);

private slots:
    void on_pushButtonChoose1stereo_clicked();

    void on_pushButtonSplit2mono_clicked();

    void show_processError();

    void on_pushButtonShowTargetInfo_clicked();

private:
    Ui::Widget *ui;
    QProcess *m_process;
    QUrl m_urlAudioFile;
};
#endif // WIDGET_H
