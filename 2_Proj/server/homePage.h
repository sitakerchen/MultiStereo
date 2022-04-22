#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QVector>
#include <QUrl>
#include <QMap>
#include <QStandardItemModel>

#include "qcombobox.h"
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
private:
QVector<QComboBox*> m_boxList;
QMap<QString, qint64> m_toChannelIndex; // channelName -> number
QVector<qint64> m_lastId; // previous status of every comboBox
qint64 m_maxSize; // max size of channel number (begain with 0

public:
bool removeItemFromAllChannelComboBox(QString text);
bool addItem2AllChannelComboBox(QString text);
void SetComboBoxItemEnabled(QComboBox * comboBox, int index, bool enabled);


signals:
    void evoke_tcpCtr_setChannel(qint64 id, qint64 channel);

private slots:
    void on_pushButtonChangeLayout_clicked();
public slots:
    void deviceList_addDevice(qint64 id); // modify comboBox and device list as client varify
    void deviceList_removeDevice(qint64 id); // modify comboBox and device list as client varify
};
#endif // HOMEPAGE_H
