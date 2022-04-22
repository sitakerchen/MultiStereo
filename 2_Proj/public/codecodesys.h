#ifndef CODECODESYS_H
#define CODECODESYS_H

#include "../public/macro.h"
#include <QObject>
#include <QMessageBox>
#include <QDebug>
#include <QTime>

class codecodeSys : public QObject
{
    Q_OBJECT
public:
    explicit codecodeSys(QObject *parent = nullptr);

private:
    static qint64 m_delayMs;
    static qint64 m_timePerIns; // 发送完一条指令要多久
    static qint64 m_clientNumber;

public:
    static void setClientNumbers(qint64 num);

public slots:
    static QString code(QString type, QString name_actObj, QString size_actName, QString num_actVal);
    static QString code_act(qint64 uAct_obj, qint64 uAct_name, qint64 uAct_val);
    static QString code_act(qint64 uAct_obj, qint64 uAct_name, QString uAct_val);

    static qint64 decode_type(QString const &ins, QString &msg_error); // type  of instruction decode

    static qint64 decode_dueTime(QString const &ins, QString &msg_error); // return the duTime of INS

    static qint64 decode_file(QString const &ins, QString &name, qint64 &uSize,
                              qint64 &num,
                              QString &msg_error); // file type ins decode

    static qint64 decode_act(QString const &ins, qint64 &uAct_obj,
                             qint64 &uAct_name, qint64 &uAct_val,
                             QString &msg_error);
    static qint64 decode_act(QString const &ins, qint64 &uAct_obj,
                             qint64 &uAct_name, QString &uAct_val,
                             QString &msg_error);

    static QString INS_generator(QString const &qstrIns);

};

#endif // CODECODESYS_H
