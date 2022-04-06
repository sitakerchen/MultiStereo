#include "tcpserver.h"
#include "ui_tcpserver.h"
using Qt::endl;

tcpserver::tcpserver(QMainWindow *parent)
    : QMainWindow(parent), ui(new Ui::tcpserver) {
  /* 初始化服务器 */
  ui->setupUi(this);
  m_tcpServer = new QTcpServer(this);

  /* initiate variables */
  m_bIsListening = false;

  /* 初始化配置 */
  setWindowTitle(tr("Server"));
  ui->lineEditIP->setText(QNetworkInterface()
                              .allAddresses()
                              .at(1)
                              .toString()); // 自动填充本机ip地址
  ui->pushButtonDisconnect->setText(tr("开始监听"));
  ui->pushButtonDisconnect->setEnabled(true);
  ui->plainTextEditRecv->setReadOnly(true);
  ui->pushButtonSend->setEnabled(false);
  ui->pushButton_sendFile->setEnabled(false);
  ui->pushButton_chooseFile->setEnabled(false);
  ui->lineEditIP->setText("192.168.123.100");
  ui->lineEditPort->setText("30000");

  connect(m_tcpServer, SIGNAL(newConnection()), this,
          SLOT(NewConnectionSlot()));
  connect(&m_outFile.timer, &QTimer::timeout, [=]() {
    //关闭定时器
    m_outFile.timer.stop();

    //发送文件
    sendData();
  });
}

tcpserver::~tcpserver() { delete ui; }

void tcpserver::sendData() {
  qint64 len = 0;
  do {
    //每次发送数据的大小 4K
    char buf[4 * 1024] = {0};
    len = 0;

    //往文件中读数据
    len = m_outFile.file.read(buf, sizeof(buf));
    //发送数据，读多少 ，发多少
    len = m_currentClient->write(buf, len);

    //发送的数据需要积累
    m_outFile.sendSize += len;

  } while (len > 0);

  //是否发送文件完毕
  if (m_outFile.sendSize == m_outFile.fileSize) {
    ui->plainTextEditSend->appendPlainText("文件发送完毕");
    m_outFile.file.close();

    //把客户端关闭
//    m_currentClient->disconnectFromHost();
//    m_currentClient->close();
  }
}

void tcpserver::ReadData() {
  for (int i = 0; i < m_ListTcpClient.length();
       ++i) // traver through every established connections
  {
    /*  recv data from client */
    QByteArray buffer = m_ListTcpClient[i]->readAll();
    if (buffer.isEmpty())
      continue;

    static QString IP_Port, IP_Port_Pre;
    IP_Port = tr("[%1:%2]:")
                  .arg(m_ListTcpClient[i]->peerAddress().toString().split(
                      "::ffff:")[1])
                  .arg(m_ListTcpClient[i]->peerPort());

    if (IP_Port != IP_Port_Pre)
      ui->plainTextEditRecv->appendPlainText(IP_Port);

    ui->plainTextEditRecv->appendPlainText(buffer);
    if (QString(buffer) == "recv finish") // file send successfully
    {
      QMessageBox::information(this, tr("File send"), tr("file send finish"));
      m_outFile.file.close();
      // cut off connection
//      m_currentClient->disconnectFromHost();
//      m_currentClient->close();
    }

    IP_Port_Pre = IP_Port;
  }
}

void tcpserver::NewConnectionSlot() {
  m_currentClient = m_tcpServer->nextPendingConnection();
  m_ListTcpClient.append(m_currentClient);
  ui->comboBox->addItem(
      tr("%1:%2")
          .arg(m_currentClient->peerAddress().toString().split("::ffff:")[1])
          .arg(m_currentClient->peerPort()));
  ui->pushButtonSend->setEnabled(true);

  /* enable only if connxion is established*/
  ui->pushButton_chooseFile->setEnabled(true);
  ui->pushButton_sendFile->setEnabled(true);

  connect(m_currentClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
  connect(m_currentClient, SIGNAL(disconnected()), this,
          SLOT(disConnectedSlot()));
}

void tcpserver::disConnectedSlot() {
  /* 由于disconnected信号没有提供请求断开连接窗口的描述信息所以需要遍历定位再断开连接并删除
   */
  for (int i = 0; i < m_ListTcpClient.length(); ++i) {
    if (m_ListTcpClient[i]->state() == QAbstractSocket::UnconnectedState) {
      ui->comboBox->removeItem(ui->comboBox->findText(
          tr("%1:%2")
              .arg(m_ListTcpClient[i]->peerAddress().toString().split(
                  "::ffff:")[1])
              .arg(m_ListTcpClient[i]->peerPort())));
      m_ListTcpClient[i]->destroyed();
      m_ListTcpClient.removeAt(i);
    }
  }
  if (m_ListTcpClient.empty()) {
    ui->pushButtonSend->setEnabled(false);
  }
}

void tcpserver::on_pushButtonDisconnect_clicked() {
  if (m_bIsListening == false) {

    /* 开始监听端口连接请求 */
    bool ok = m_tcpServer->listen(QHostAddress::Any,
                                  ui->lineEditPort->text().toInt());
    if (ok) {
      ui->pushButtonDisconnect->setText("断开");

      /* 开始监听成功 */
      m_bIsListening = true;
    } else {
      QMessageBox::critical(this, tr("Server error"),
                            tr("fail to listening to port %1")
                                .arg(ui->lineEditPort->text().toInt()));
    }
    return;
  }

  /* 关闭所有连接并停止监听窗口 */
  // 有问题，会崩溃：考虑只保留disconnectFromHost其他删掉
  for (int i = 0; i < m_ListTcpClient.length(); ++i) {
    m_ListTcpClient[i]->disconnectFromHost();
    if (m_ListTcpClient[i]->state() == QAbstractSocket::UnconnectedState or
        m_ListTcpClient[i]->waitForDisconnected(1000)) {
      QMessageBox::information(this, tr("Success"), tr("disconnect success"));
    } else {
      // exception handle
      QMessageBox::critical(this, tr("Error"), tr("disconnected fail"));
    }
  }
  m_ListTcpClient.clear();
  m_tcpServer->close();

  m_bIsListening = false;
  ui->pushButtonSend->setEnabled(false);
  ui->pushButtonDisconnect->setText(tr("开始监听"));
}

void tcpserver::on_pushButtonClearWindow_clicked() {
  ui->plainTextEditRecv->setReadOnly(false);
  ui->plainTextEditRecv->clear();
  ui->plainTextEditRecv->setReadOnly(true);
}

void tcpserver::on_pushButtonSend_clicked() {
  QString qsSend = ui->plainTextEditSend->toPlainText();
  //    /* 向所有连接的客户端广播消息 */
  //    if (ui->comboBox->currentIndex() == 0)
  //    {
  //        for (int i = 0 ; i < tcpClient.length() ; ++ i)
  //            tcpClient[i]->write(data.toLatin1());
  //    }
  if (qsSend.length() <= 0 or
      ui->comboBox->count() ==
          0) // 如果发送窗口没有消息，或者没有已经建立的连接
  {
    QMessageBox::critical(
        this, tr("Error"),
        tr("send message is empty or no connection avaliable"));
    return;
  }

  /* 向指定客户端发送消息 */
  QString clientIP = ui->comboBox->currentText().split(":")[0];
  qDebug() << "clienIP :" << clientIP << endl;
  int clientPort = ui->comboBox->currentText().split(":")[1].toInt();
  qDebug() << "clienPort :" << clientPort << endl;

  for (int i = 0; i < m_ListTcpClient.length(); ++i) {
    qDebug() << m_ListTcpClient[i]->peerAddress().toString().split("::ffff:")[1]
             << endl;
    if (m_ListTcpClient[i]->peerAddress().toString().split("::ffff:")[1] ==
            clientIP and
        m_ListTcpClient[i]->peerPort() == clientPort) {
      m_ListTcpClient[i]->write(qsSend.toLatin1());
      return;
    }
  }
}

void tcpserver::on_pushButton_chooseFile_clicked() {
  QString filePath = QFileDialog::getOpenFileName(this, "open", "../");

  if (false == filePath.isEmpty()) {
    m_outFile.fileName.clear();
    m_outFile.fileSize = 0;

    //获取文件信息
    QFileInfo info(filePath);
    m_outFile.fileName = info.fileName(); //获取文件名字
    m_outFile.fileSize = info.size();     //获取文件大小

    m_outFile.sendSize = 0; //发送文件的大小

    //只读方式打开文件
    //指定文件的名字
    m_outFile.file.setFileName(filePath);

    //打开文件
    bool isOk = m_outFile.file.open(QIODevice::ReadOnly);
    if (false == isOk) {
      qDebug() << "只读方式打开文件失败 78";
    }

    //提示打开文件路径
    ui->lineEdit_FileName->setText(filePath);

    ui->pushButton_chooseFile->setEnabled(false);
    ui->pushButton_sendFile->setEnabled(true);
  } else {
    qDebug() << "选择文件路径出错 62 ";
  }
}

void tcpserver::on_pushButton_sendFile_clicked() {
  //先发送文件头信息  格式如：文件名##文件大小
  QString head =
      QString("%1##%2").arg(m_outFile.fileName).arg(m_outFile.fileSize);

  //发送头部的信息
  qint64 len = m_currentClient->write(head.toUtf8());

  if (len > 0) //头部信息发送成功
  {
    //发送真正的文件信息
    //防止TCP黏包文件
    //需要通过定时器延时 20ms
    m_outFile.timer.start(20);
  } else {
    qDebug() << "头部信息发送失败 114";
    m_outFile.file.close();
    ui->pushButton_chooseFile->setEnabled(true);
    ui->pushButton_sendFile->setEnabled(false);
  }
}
