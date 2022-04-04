#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <tcpclient.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // page button
    void on_pushButton_tcpSetting_clicked();
    void on_pushButton_music_clicked();
    void on_pushButton_main_clicked();

private:
    Ui::MainWindow *ui;
    tcpclient *m_client;
};
#endif // MAINWINDOW_H
