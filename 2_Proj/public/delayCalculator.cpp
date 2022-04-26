#include "delayCalculator.h"

calculator::Release calculator::m_MemRelease;
calculator *calculator::m_calculator;
QElapsedTimer calculator::m_delayTimer;

calculator::calculator(QObject *parent)
    : QObject{parent}
{

}

calculator &calculator::getInstance()
{
    if (m_calculator) return *m_calculator;
    std::once_flag oc;
    std::call_once(oc, [&]{m_calculator = new calculator();});
    return *m_calculator;
}

qint64 calculator::baseDelayTime()
{
    return m_baseDelayTime;
}

qint64 calculator::currentTime_ms()
{
    return QTime::currentTime().msecsSinceStartOfDay();
}

qint64 calculator::playDelay_client(qint64 baseDelay, qint64 sendTime, qint64 sendDelay, qint64 recvDelay)
{
//    qint64 nTransDelay = QTime::currentTime().msecsSinceStartOfDay() - sendTime;
    qint64 nDelay = baseDelay - sendDelay - recvDelay;
    if (nDelay < 0) return 0;
    else return nDelay;
}

qint64 calculator::playDelay_server(qint64 allInsSendDelay)
{
    qDebug() <<"all send delay = " << allInsSendDelay << Qt::endl;
    qint64 nDelay = m_baseDelayTime - allInsSendDelay;
    if (nDelay < 0) return 0;
    else return nDelay;
}

void calculator::startDelayTimer()
{
    m_delayTimer.restart();
}

qint64 calculator::getDelayTime()
{
    return m_delayTimer.elapsed();
}

void calculator::setBaseDelayTime(qint64 delayTime_ms)
{
    if (delayTime_ms == m_baseDelayTime) return;
    m_baseDelayTime = delayTime_ms;
    emit baseDelayTimeChanged(delayTime_ms);
}
