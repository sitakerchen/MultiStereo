#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void show_tcpClient();
    void show_musicPlayer();

private:
    Ui::MainWindow *ui;

public slots:

    // page button
    void ShowMyself(); // show page

    void on_pushButton_tcpSetting_clicked();
    void on_pushButton_music_clicked();
    void on_pushButton_main_clicked();
};
#endif // MAINWINDOW_H
