#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QUrl>

#include "tcpController.h"
#include "rawmediaprocess.h"
#include "processcontrol.h"
#include "../public/macro.h"

QT_BEGIN_NAMESPACE
namespace Ui { class homePage; }
QT_END_NAMESPACE

class homePage : public QWidget
{
    Q_OBJECT

public:
    homePage(QWidget *parent = nullptr);
    ~homePage() override;
private:
    Ui::homePage *ui;

    /* tcp controller */
private:
    tcpController *m_tcpCtr;

private slots:
    void on_pushButtonController_clicked();

    /* media process */
private:
    rawMediaProcess *m_MedaProces;

private slots:
    void on_pushButtonMediaProcess_clicked();

    /* channel settings */
private slots:
    void on_pushButtonChangeLayout_clicked();
};
#endif // HOMEPAGE_H
