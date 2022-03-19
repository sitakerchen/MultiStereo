#include "tcpserver.h"
#include "ui_tcpserver.h"
using Qt::endl;

tcpserver::tcpserver(QMainWindow *parent)
    : QMainWindow(parent)
    , ui(new Ui::tcpserver)
{
    /* 初始化服务器 */
    ui->setupUi(this);
    m_tcpServer = new QTcpServer(this);

    /* 初始化配置 */
    setWindowTitle(tr("Server"));
    ui->lineEditIP->setText(QNetworkInterface().allAddresses().at(1).toString()); // 自动填充本机ip地址
    ui->pushButtonDisconnect->setText(tr("开始监听"));
    ui->pushButtonDisconnect->setEnabled(true);
    ui->plainTextEditRecv->setReadOnly(true);
    ui->pushButtonSend->setEnabled(false);
    m_bIsListening = false;

    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(NewConnectionSlot()));
}

tcpserver::~tcpserver()
{
    delete ui;
}

void tcpserver::ReadData()
{
    for (int i = 0 ; i < m_tcpClient.length() ; ++ i)
    {
        QByteArray buffer = m_tcpClient[i]->readAll();
        if (buffer.isEmpty()) continue;

        static QString IP_Port, IP_Port_Pre;
        IP_Port = tr("[%1:%2]:").arg(m_tcpClient[i]->peerAddress().toString().split("::ffff:")[1])\
                .arg(m_tcpClient[i]->peerPort());

        if (IP_Port != IP_Port_Pre)
            ui->plainTextEditRecv->appendPlainText(IP_Port);

        ui->plainTextEditRecv->appendPlainText(buffer);

        IP_Port_Pre = IP_Port;
    }
}

void tcpserver::NewConnectionSlot()
{
    m_currentClient = m_tcpServer->nextPendingConnection();
    m_tcpClient.append(m_currentClient);
    ui->comboBox->addItem(tr("%1:%2").arg(m_currentClient->peerAddress().toString().split("::ffff:")[1]).arg(m_currentClient->peerPort()));
    ui->pushButtonSend->setEnabled(true);

    connect(m_currentClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    connect(m_currentClient, SIGNAL(disconnected()), this, SLOT(disConnectedSlot()));
}

void tcpserver::disConnectedSlot()
{
    /* 由于disconnected信号没有提供请求断开连接窗口的描述信息所以需要遍历定位再断开连接并删除 */
    for (int i = 0 ; i < m_tcpClient.length() ; ++ i)
    {
        if (m_tcpClient[i]->state() == QAbstractSocket::UnconnectedState)
        {
            ui->comboBox->removeItem(ui->comboBox->findText(tr("%1:%2")\
                                                            .arg(m_tcpClient[i]->peerAddress().toString().split("::ffff:")[1])\
                                     .arg(m_tcpClient[i]->peerPort())));
            m_tcpClient[i]->destroyed();
            m_tcpClient.removeAt(i);
        }
    }
    if (m_tcpClient.empty())
    {
        ui->pushButtonSend->setEnabled(false);
    }
}

void tcpserver::on_pushButtonDisconnect_clicked()
{
    if (m_bIsListening == false)
    {

        /* 开始监听端口连接请求 */
        bool ok = m_tcpServer->listen(QHostAddress::Any, ui->lineEditPort->text().toInt());
        if (ok)
        {
            ui->pushButtonDisconnect->setText("断开");

            /* 开始监听成功 */
            m_bIsListening = true;
        }
        else
        {
            QMessageBox::critical(this, tr("Server error"), tr("fail to listening to port %1").arg(ui->lineEditPort->text().toInt()));
        }
        return ;
    }

    /* 关闭所有连接并停止监听窗口 */
    // 有问题，会崩溃：考虑只保留disconnectFromHost其他删掉
    for (int i = 0 ; i < m_tcpClient.length() ; ++ i)
    {
        m_tcpClient[i]->disconnectFromHost();
        if (m_tcpClient[i]->state() == QAbstractSocket::UnconnectedState or m_tcpClient[i]->waitForDisconnected(1000))
        {
            QMessageBox::information(this, tr("Success"), tr("disconnect success"));
        }
        else
        {
            // exception handle
            QMessageBox::critical(this, tr("Error"), tr("disconnected fail"));
        }
    }
    m_tcpClient.clear();
    m_tcpServer->close();

    m_bIsListening = false;
    ui->pushButtonSend->setEnabled(false);
    ui->pushButtonDisconnect->setText(tr("开始监听"));
}

void tcpserver::on_pushButtonClearWindow_clicked()
{
    ui->plainTextEditRecv->setReadOnly(false);
    ui->plainTextEditRecv->clear();
    ui->plainTextEditRecv->setReadOnly(true);
}

void tcpserver::on_pushButtonSend_clicked()
{
    QString qsSend = ui->plainTextEditSend->toPlainText();
//    /* 向所有连接的客户端广播消息 */
//    if (ui->comboBox->currentIndex() == 0)
//    {
//        for (int i = 0 ; i < tcpClient.length() ; ++ i)
//            tcpClient[i]->write(data.toLatin1());
//    }
     if (qsSend.length() <= 0 or ui->comboBox->count() == 0) // 如果发送窗口没有消息，或者没有已经建立的连接
     {
         QMessageBox::critical(this, tr("Error"), tr("send message is empty or no connection avaliable"));
        return ;
     }

    /* 向指定客户端发送消息 */
    QString clientIP = ui->comboBox->currentText().split(":")[0];
    qDebug() << "clienIP :" << clientIP << endl;
    int clientPort = ui->comboBox->currentText().split(":")[1].toInt();
    qDebug() << "clienPort :" << clientPort << endl;


    for (int i = 0 ; i < m_tcpClient.length() ; ++ i)
    {
        qDebug() << m_tcpClient[i]->peerAddress().toString().split("::ffff:")[1] <<endl;
        if ( m_tcpClient[i]->peerAddress().toString().split("::ffff:")[1] == clientIP and m_tcpClient[i]->peerPort() == clientPort)
        {
            m_tcpClient[i]->write(qsSend.toLatin1());
            return ;
        }
    }
}
