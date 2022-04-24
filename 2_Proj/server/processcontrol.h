#ifndef PROCESSCONTROL_H
#define PROCESSCONTROL_H

#include <QObject>
#include <QProcess>
#include <mutex>
#include <QMessageBox>
#include <QDir>

class ProcessControl : public QObject
{
    Q_OBJECT
public:
    explicit ProcessControl(QObject *parent = nullptr);

    static ProcessControl& getInstance();

    bool setWorkingPath(QString qstrPath = QDir::currentPath());
    void writeCommand(QString qstrCmd);
    bool wait_forFinished(); // 方便进程同步，但是这玩意很慢不能凡在写函数里！！！

public slots:
    QString ErrorInfo();
    QString OutPutInfo();

signals:
    void sendErrorInfo(QString msg);
    void sendOutPutInfo(QString msg);

private:
    static ProcessControl *m_controller; // instance of this class

    QProcess *m_process; // process instance

    /* release heap(new) storage */
    class Release
        {
        public:
            ~Release()
            {
                if (ProcessControl::m_controller == nullptr) return;

                /* release m_process */
                ProcessControl::m_controller->m_process->close();
                ProcessControl::m_controller->m_process->waitForFinished();
                if (ProcessControl::m_controller->m_process)
                {
                    delete ProcessControl::m_controller->m_process;
                    ProcessControl::m_controller->m_process = nullptr;
                }

                /* release ProcessControl::m_controller */
                if (ProcessControl::m_controller)
                {
                    delete ProcessControl::m_controller;
                    ProcessControl::m_controller = nullptr;
                }
                qDebug() << "process realse finished" << Qt::endl;
            }
        };
        static Release m_MemRelease;
};

#endif // PROCESSCONTROL_H
