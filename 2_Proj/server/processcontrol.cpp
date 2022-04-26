#include "processcontrol.h"
using Qt::endl;

ProcessControl *ProcessControl::m_controller;
ProcessControl::Release ProcessControl::m_MemRelease;

ProcessControl::ProcessControl(QObject *parent)
    : QObject{parent}
{
    /* initiate cmd/terminal process */
    m_process = new QProcess(this);
#ifdef Q_OS_WIN
    m_process->start("cmd");
#else
    m_process->start("zsh");
#endif
    m_process->waitForStarted(); // wait for process start

//    m_process->setProcessChannelMode(QProcess::MergedChannels); // detail in help document
}

// return instance of ProcessControl
ProcessControl& ProcessControl::getInstance()
{
    if (m_controller)
    {
        return *m_controller;
    }
    std::once_flag oc;
    std::call_once(oc, [&]{m_controller = new ProcessControl();});

    /* connect */
    connect(ProcessControl::getInstance().m_process, &QProcess::readyReadStandardOutput, &ProcessControl::getInstance(), &ProcessControl::OutPutInfo);
    connect(ProcessControl::getInstance().m_process, &QProcess::readyReadStandardError, &ProcessControl::getInstance(), &ProcessControl::ErrorInfo);

    return *m_controller;
}

/* : set proces's current working path
 * [in]:
 *      QString astrPath: absolute path, default = QDir::currentPath()
 *
 * [return]:
 *          true: if the given path exists
 *          false: if the given path not exists
 */
bool ProcessControl::setWorkingPath(QString qstrPath)
{
    QDir dirPath(qstrPath);
    if (dirPath.exists() == false)
    {
        qDebug() << " path not exist" << endl;
        return false;
    }

#ifdef Q_OS_WIN
    QString ROOT_OF_FILE = qstrPath.mid(0, 2);
    qDebug() << "root : " << ROOT_OF_FILE << endl;
    writeCommand(ROOT_OF_FILE); // change root
#endif

    QString ch2CurPath = tr("cd %1").arg( QDir::toNativeSeparators(qstrPath));
    qDebug() << "current path : " << ch2CurPath << endl;
    writeCommand(ch2CurPath); // change to current path
    return true;
}

/* :write command in terminal or cmd
 * [in]:
 *  QString cmd: the command that write to terminal/cmd, this argument are not supposed to end with '\r' or '\r\n' which will automatically added in this method.
 *
 * return void
 */
void ProcessControl::writeCommand(QString qstrCmd)
{
    qstrCmd += "\r\n"; // indispensable
    QByteArray qbCmd = qstrCmd.toUtf8();
    char * szCmd = qbCmd.data();
    long long nlen = qstrCmd.length();
    m_process->write(szCmd, nlen);
}

bool ProcessControl::wait_forFinished()
{
    return m_process->waitForFinished();
}

// [slot]show error info in the means of pop-up window if an error occured during the process running
QString ProcessControl::ErrorInfo()
{
    QString qsErrorInfo = QString::fromLocal8Bit(m_process->readAllStandardError());
    if (qsErrorInfo.length() <= 0) return "";
    emit sendErrorInfo(qsErrorInfo);
    return qsErrorInfo;
}

// [slot]display std outPut from process
QString ProcessControl::OutPutInfo()
{
    QString qsOutPutInfo = QString::fromLocal8Bit(m_process->readAllStandardOutput());
    if (qsOutPutInfo.length() <= 0) return "";
    emit sendOutPutInfo(qsOutPutInfo);
    return qsOutPutInfo;
}

