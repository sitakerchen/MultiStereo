#ifndef DELAYCALCULATOR_H
#define DELAYCALCULATOR_H

#include <mutex>
#include <QObject>
#include <QTime>
#include <QDebug>
#include <QElapsedTimer>

class calculator : public QObject
{
    Q_OBJECT
public:
    explicit calculator(QObject *parent = nullptr);
    static calculator& getInstance();

private:
    static calculator *m_calculator;
    static QElapsedTimer m_delayTimer; // 发送或接收时延计时器
    qint64 m_baseDelayTime = 120; // 基准延时

public:
    Q_PROPERTY(qint64 baseDelayTime READ baseDelayTime WRITE setBaseDelayTime NOTIFY baseDelayTimeChanged)
    qint64 baseDelayTime(); // get基准延时
    qint64 currentTime_ms(); // get current milliSeconds from 00:00
    qint64 playDelay_client(qint64 baseDelay, qint64 sendTime, qint64 sendDelay, qint64 recvDelay);
    qint64 playDelay_server(qint64 allInsSendDelay);
    void startDelayTimer(); // restart the timer, ms
    qint64 getDelayTime(); // get elapse time since last restart, ms

public slots:
    void setBaseDelayTime(qint64 delayTime_ms);

signals:
    void baseDelayTimeChanged(qint64 newVal);

private:
    class Release
        {
        public:
            ~Release()
            {
                if (calculator::m_calculator)
                {
                    delete calculator::m_calculator;
                    calculator::m_calculator = nullptr;
                }
                qDebug() << "process realse finished" << Qt::endl;
            }
        };
        static Release m_MemRelease;
};

#endif // DELAYCALCULATOR_H
