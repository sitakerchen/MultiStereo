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

public slots:
    void ErrorInfo();
    void OutPutInfo();

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
            }
        };
        static Release m_MemRelease;
};

#endif // PROCESSCONTROL_H