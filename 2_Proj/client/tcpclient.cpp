#include "tcpclient.h"
#include "ui_tcpclient.h"

tcpclient::tcpclient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::tcpclient)
{
    ui->setupUi(this);

    /* initialize TCP client */
    tcpClient = new QTcpSocket(this); // instance of tcpClient
    tcpClient->abort(); // cancel original connexion

    /* 连接信号与槽 */
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData())); // 连接准备读就绪信号与读数据槽函数
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(ReadError(QAbstractSocket::SocketError))); // 连接错误信号与错误处理槽函数

    /* 初始化设置 */
    setWindowTitle(tr("Client"));
    ui->plainTextEditRecv->setReadOnly(true); // 接收窗口设为只读
    ui->pushButtonSend->setEnabled(false); // 未连接状态下无法发送数据
    ui->pushButtonDisconnect->setText(tr("连接"));
    isConnected = false; // 初始化为未连接状态
}

tcpclient::~tcpclient()
{
    delete ui;
}

void tcpclient::ReadData()
{
    /* 读取服务器数据并显示 */
    QByteArray buffer = tcpClient->readAll();
    if (!buffer.isEmpty())
    {
        ui->plainTextEditRecv->appendPlainText(buffer);
    }
}

void tcpclient::ReadError(QAbstractSocket::SocketError)
{
    /* 处理TCP连接错误 */
    tcpClient->disconnectFromHost(); // 先断开TCP连接
    ui->pushButtonDisconnect->setText(tr("连接"));
    ui->pushButtonSend->setEnabled(false);

    QMessageBox msgBox;
    msgBox.setText(tr("failed to connect server because %1").arg(tcpClient->errorString())); // 弹出错误消息窗口

}

void tcpclient::on_pushButtonDisconnect_clicked()
{
    if (isConnected == false)
    {
        /* 尝试连接服务器 */
        tcpClient->connectToHost(ui->lineEditIP->text(), ui->lineEditPort->text().toInt());
        QString msg("trying to connect to the host %1 at port %2");
        qDebug() << msg.arg(ui->lineEditIP->text()).arg(ui->lineEditPort->text());

        /* 等待连接 */
        if (tcpClient->waitForConnected(10000))
        {

        /* 连接成功 */
            qDebug() << "connected!" << Qt::endl;
            ui->pushButtonDisconnect->setText(tr("断开"));
            ui->pushButtonDisconnect->setEnabled(true);
            ui->pushButtonSend->setEnabled(true);
            isConnected = true;
        }
        else
        {
        /* 连接失败 */
            QMessageBox::critical(this, tr("connect error"), tr("fail to connect to the server"));
        }
        return ;
    }

    /* 断开与服务器连接 */
    tcpClient->disconnectFromHost();
    if (tcpClient->state() == QAbstractSocket::UnconnectedState or tcpClient->waitForDisconnected(10000)) // 如果处于未连接状态或者等待一段时间后成功断开连接
    {
        ui->pushButtonDisconnect->setText("连接");
        ui->pushButtonSend->setEnabled(false);
    }
    isConnected = false;
}

void tcpclient::on_pushButtonClearWindow_clicked()
{
    /* 清除接收窗口 */
    ui->plainTextEditRecv->setReadOnly(false);
    ui->plainTextEditRecv->clear();
    ui->plainTextEditRecv->setReadOnly(true);
}

void tcpclient::on_pushButtonSend_clicked()
{
    /* 发送数据到服务器端 */
    QString data = ui->plainTextEditSend->toPlainText();
    if (data != "")
    {
        tcpClient->write(data.toLatin1());
    }
}
