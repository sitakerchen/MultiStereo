#include "tcpclient.h"
#include "ui_tcpclient.h"

tcpclient::tcpclient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::tcpclient)
{
    ui->setupUi(this);

    /* initialize TCP client */
    m_tcpClient = new QTcpSocket(this); // instance of tcpClient
    m_tcpClient->abort(); // cancel original connexion

    /* 连接信号与槽 */
    connect(m_tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData())); // 连接准备读就绪信号与读数据槽函数
    connect(m_tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(ReadError(QAbstractSocket::SocketError))); // 连接错误信号与错误处理槽函数

    /* 初始化variables */
    setWindowTitle(tr("Client"));
    m_isConnected = false; // 初始化为未连接状态
    m_isStart = true;

    /* initiate widget */
    ui->progressBar->setValue(0);
    ui->plainTextEditRecv->setReadOnly(true); // 接收窗口设为只读
    ui->pushButtonDisconnect->setText(tr("连接"));
}

tcpclient::~tcpclient() { delete ui; }

void tcpclient::ReadData()
{
   /* debug messages */
//    /* 读取服务器数据并在文本窗口显示 */
//    QByteArray buffer = m_tcpClient->readAll();
//    if (!buffer.isEmpty())
//    {
//        ui->plainTextEditRecv->appendPlainText(buffer);
//    }

    /* recv media file */
    //取出接收的内容
    QByteArray buf = m_tcpClient->readAll();
    if (true == m_isStart) {
      //接收头
      m_isStart = false;

      //            分段   0   0  1  1  2    2
      //解析头部信息 buf = "hello##1024##333333"
      //            QString str = "hello##1024";
      //            str.section("##",0,0);//这个拆出来hello，其中##是分段标识符，格式为：段1+标识符+段2+标识符+段3+标识符+……

      //初始化
      m_mdiFile.fileName = QString(buf).section("##", 0, 0);
      m_mdiFile.fileSize = QString(buf).section("##", 1, 1).toInt();
      m_mdiFile.recvSize = 0;

      //打开文件
      m_mdiFile.file.setFileName(m_mdiFile.fileName);

      bool isOk = m_mdiFile.file.open(QIODevice::WriteOnly);
      if (false == isOk) {
        qDebug() << "WriteOnly error 38 ";

        m_tcpClient->disconnectFromHost(); //断开连接
        m_tcpClient->close();              //关闭套接字

        return; //如果打开文件失败，中断函数
      }
      //弹出对话框，显示接收文件信息

      QString str =
          QString("接收文件： [%1 : %2kb]").arg(m_mdiFile.fileName).arg(m_mdiFile.fileSize / 1024);
      QMessageBox::information(this, "文件信息", str);

      //设置进度条
      ui->progressBar->setMinimum(0);               //最小值
      ui->progressBar->setMaximum(m_mdiFile.fileSize / 1024); //最大值
      ui->progressBar->setValue(0);                 //当前值

    } else //文件信息
    {
      qint64 len = m_mdiFile.file.write(buf);

      if (len > 0) {
        m_mdiFile.recvSize += len; //累积接收大小
        qDebug() << len;
      }
      //更新进度条
      ui->progressBar->setValue(m_mdiFile.recvSize / 1024);

      if (m_mdiFile.recvSize == m_mdiFile.fileSize) {
        //先给服务发送（接收文件完成的消息）
        m_tcpClient->write("file done ");

        QMessageBox::information(this, "完成", "文件接收完毕");

        m_mdiFile.file.close(); //关闭文件

        //断开连接
        m_tcpClient->disconnectFromHost();
        m_tcpClient->close();
      }
    }
}

void tcpclient::ReadError(QAbstractSocket::SocketError)
{
    /* 处理TCP连接错误 */
    m_tcpClient->disconnectFromHost(); // 先断开TCP连接
    ui->pushButtonDisconnect->setText(tr("连接"));

    QMessageBox msgBox;
    msgBox.setText(tr("failed to connect server because %1").arg(m_tcpClient->errorString())); // 弹出错误消息窗口

}

void tcpclient::SendData(QString data)
{
    /* 发送数据到服务器端 */
    if (data != "")
    {
        m_tcpClient->write(data.toLatin1());
    }
}


void tcpclient::on_pushButtonDisconnect_clicked()
{
    if (m_isConnected == false)
    {
        /* 尝试连接服务器 */
        m_tcpClient->connectToHost(ui->lineEditIP->text(), ui->lineEditPort->text().toInt());
        QString msg("trying to connect to the host %1 at port %2");
        qDebug() << msg.arg(ui->lineEditIP->text()).arg(ui->lineEditPort->text());

        /* 等待连接 */
        if (m_tcpClient->waitForConnected(10000))
        {

        /* 连接成功 */
            qDebug() << "connected!" << Qt::endl;
            ui->pushButtonDisconnect->setText(tr("断开"));
            ui->pushButtonDisconnect->setEnabled(true);
            m_isConnected = true;
        }
        else
        {
        /* 连接失败 */
            QMessageBox::critical(this, tr("connect error"), tr("fail to connect to the server"));
        }
        return ;
    }

    /* 断开与服务器连接 */
    m_tcpClient->disconnectFromHost();
    if (m_tcpClient->state() == QAbstractSocket::UnconnectedState or m_tcpClient->waitForDisconnected(10000)) // 如果处于未连接状态或者等待一段时间后成功断开连接
    {
        ui->pushButtonDisconnect->setText("连接");
    }
    m_isConnected = false;
}

void tcpclient::on_pushButtonClearWindow_clicked()
{
    /* 清除接收窗口 */
    ui->plainTextEditRecv->setReadOnly(false);
    ui->plainTextEditRecv->clear();
    ui->plainTextEditRecv->setReadOnly(true);
}

void tcpclient::on_pushButton_main_clicked()
{
    this->hide();
}


void tcpclient::on_pushButton_tcpSetting_clicked()
{

}


void tcpclient::on_pushButton_music_clicked()
{
    // jump to music ui
}

