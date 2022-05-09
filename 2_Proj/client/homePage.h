#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include "../public/macro.h"
#include "../public/codecodesys.h"

QT_BEGIN_NAMESPACE
namespace Ui { class homePage; }
QT_END_NAMESPACE

class homePage : public QMainWindow
{
    Q_OBJECT

public:
    homePage(QWidget *parent = nullptr);
    ~homePage() override;

private:
    Ui::homePage *ui;

    /* INS process */
public slots:
    void ins_process(qint64 uAct_name, QString uAct_val);

    /* status display */
public slots:
    void setId(qint64 id);
    void setStatus(bool stat);
    void setChannel(QString channel);

    /* page jump button */
signals:
void show_tcpClient();
void show_musicPlayer();

public slots:
    void ShowMyself(); // show page
    void on_pushButton_tcpSetting_clicked();
    void on_pushButton_music_clicked();
    void on_pushButton_main_clicked();
};
#endif // HOMEPAGE_H
