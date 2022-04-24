#include "codecodesys.h"
using Qt::endl;

codecodeSys::codecodeSys(QObject *parent)
    : QObject{parent}
{

}

// prefix = 5位10进制数+#
QString codecodeSys::INS_generator(QString const &qstrIns)
{
    /* add a time section to the tail of INS */
    calculator &cal = calculator::getInstance();
    QString qstrBaseDelay = "##" + QString::number(cal.baseDelayTime());
    QString qstrSendTime = "##" +  QString::number(QTime::currentTime().msecsSinceStartOfDay());
    QString qstrSendDelay = "##" + QString::number(cal.getDelayTime());


    /* calculate length */
    qint64 nlen = qstrIns.length() + qstrBaseDelay.length() + qstrSendTime.length() + qstrSendDelay.length();
    if (nlen > 99999)
    {
        QMessageBox::critical(nullptr, tr("INS error"), tr("instruction too long!!"));
        return "";
    }

    /* add prefix(指令的总长度，5位10进制数，不计入总长度) */
    QString prefix = QString("%1").arg(nlen, 5, 10, QLatin1Char('0')) + "#";

    return prefix + qstrIns + qstrBaseDelay + qstrSendTime  + qstrSendDelay;
}


/*
 *  in:
 *      sec1(qstr):   type of instruction: 0/1/2
 *      sec2(qStr):   name/actionObject/msg
 *      sec3(qstr):   fileSize/actionName/msg
 *      sec4(qstr):   channelNumber/actionValue/msg
 */
QString codecodeSys::code(QString type, QString name_actObj, QString size_actName, QString num_actVal)
{
    // TODO: need complement
    // COMMENT: reduce complexity, deprecate judgment code here
//    bool ok1, ok2;
//    num_actVal.toInt(&ok1);
//    qint32 uType = type.toInt(&ok2);
//    QString qstrMsg_error;
//    if (uType <0 or uType > 2)
//    {
//        ok2 = false;
//    }
//    if (!ok1 or !ok2)
//    {
//        QMessageBox::critical(nullptr, tr("instruction"), tr("invalid instruction\n%1").arg(qstrMsg_error));
//        return nullptr;
//    }
    QString qstrRes = INS_.arg(type, name_actObj, size_actName, num_actVal);
    return qstrRes;
}

QString codecodeSys::code_act(qint64 uAct_obj, qint64 uAct_name,qint64 uAct_val )
{
    QString qstrIns = INS_.arg(TYPE_ACT).arg(uAct_obj).arg(uAct_name).arg(uAct_val);
    qDebug() << "generate act INS: " << qstrIns << endl;
    return qstrIns;
}

QString codecodeSys::code_act(qint64 uAct_obj, qint64 uAct_name, QString uAct_val)
{
    QString qstrIns = INS_.arg(TYPE_ACT).arg(uAct_obj).arg(uAct_name).arg(uAct_val);
    qDebug() << "generate act INS: " << qstrIns << endl;
    return qstrIns;
}

qint64 codecodeSys::decode_type(QString const &ins, QString &msg_error)
{
    qint64 uType = ins.section("##", 0, 0).toInt();
    if (uType < 0 or uType > 2)
    {
        msg_error = "invalid instruction type!";
        qDebug() << "INS: " << ins << endl;
        return -1;
    }
    return uType;
}

/*
 * in:
 *      ins
 *
 * out:
 *      baseDelay
 *      sendTime
 *      sendDelay
 *      msg_error
 */
qint64 codecodeSys::decode_delayTime(QString const &ins, qint64 &baseDelay, qint64 &sendTime, qint64 &sendDelay, QString &msg_error)
{
    bool ok1, ok2, ok3;
    baseDelay = ins.section("##", 4, 4).toLongLong(&ok1);
    sendTime = ins.section("##", 5, 5).toLongLong(&ok2);
    sendDelay = ins.section("##", 6, 6).toLongLong(&ok3);
    if (ok1 and ok2 and ok3)
    {
        return 0;
    }
    else
    {
        msg_error = "delay time conversion error";
        qDebug() << msg_error << endl;
        return -1;
    }
}

/*
 *  in:
 *      ins(qstr):     instruction
 *      name(qstr):    name of file
 *      uSize(qint32): size of file
 *      num(qint32):   index of channel
 *  out:
 *      msg_error(qstr): error message
 *  return:
 *          a number(qint32) which denote the type of instruction
 *          if error occured, return -1
 */
qint64 codecodeSys::decode_file(QString const &ins, QString &name, qint64 &uSize, qint64 &num, QString &msg_error)
{
    name  = ins.section("##", 1, 1);
    uSize = ins.section("##", 2, 2).toInt();
    num   = ins.section("##", 3, 3).toInt();
    if (num != 2 and num != 5)
    {
        msg_error = "undefined number of channels";
        qDebug() << "INS: " << ins << endl;
        return -1;
    }
    return 0;
}

/*
 *  in:
 *  out:
 *  return:
 *          a number(qint32) which denote the type of instruction
 *          if error occured, return -1
 */
qint64 codecodeSys::decode_act(QString const &ins, qint64 &uAct_obj, qint64 &uAct_name, qint64 &uAct_val, QString &msg_error)
{
    bool ok1, ok2, ok3 = true;
    uAct_obj =  ins.section("##", 1, 1).toInt(&ok1);
    uAct_name = ins.section("##", 2, 2).toInt(&ok2);
    uAct_val =  ins.section("##", 3, 3).toInt(&ok3);
    if (ok1 and ok2 and ok3)
    {
        return 0;
    }
    msg_error = "invalid action obj or name or value";
    qDebug() << "INS: " << ins << endl;
    return -1;
}

/*
 *  in:
 *  out:
 *  return:
 *          a number(qint32) which denote the type of instruction
 *          if error occured, return -1
 */
qint64 codecodeSys::decode_act(const QString &ins, qint64 &uAct_obj, qint64 &uAct_name, QString &uAct_val, QString &msg_error)
{
     bool ok1, ok2, ok3 = true;
    uAct_obj =  ins.section("##", 1, 1).toInt(&ok1);
    uAct_name = ins.section("##", 2, 2).toInt(&ok2);
    uAct_val =  ins.section("##", 3, 3);
    if (ok1 and ok2 and ok3)
    {
        return 0;
    }
    msg_error = "invalid action obj or name or value";
    qDebug() << "INS: " << ins << endl;
    return -1;
}
