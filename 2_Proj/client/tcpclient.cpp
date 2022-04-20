#include "tcpclient.h"
#include "ui_tcpclient.h"
using Qt::endl;

tcpclient::tcpclient(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::tcpclient) {
  ui->setupUi(this);

  /* initialize TCP client */
  m_tcpClient = new QTcpSocket(this); // instance of tcpClient
  m_tcpClient->abort();               // cancel original connexion

  /* 初始化variables */
  setWindowTitle(tr("Client"));
  m_isConnected = false; // 初始化为未连接状态
  Reset_fileRecvStatus();


  create_homeDir();

  /* initiate widget */
  ui->progressBar->setValue(0);             // set progress bar initial value
  ui->plainTextEditRecv->setReadOnly(true); // 接收窗口设为只读
  QScroller::grabGesture(ui->plainTextEditRecv, QScroller::TouchGesture);
  ui->plainTextEditRecv->setTextInteractionFlags (Qt::NoTextInteraction);
  ui->pushButtonDisconnect->setText(tr("连接"));
  ui->lineEditIP->setText("192.168.123.100");
  ui->lineEditPort->setText("30000");

  /* 连接信号与槽 */
  connect(m_tcpClient, &QIODevice::readyRead, this,
          &tcpclient::ReadData); // 连接准备读就绪信号与读数据槽函数
  connect(m_tcpClient, &QTcpSocket::errorOccurred, this,
          &tcpclient::ReadError); // 连接错误信号与错误处理槽函数
}

tcpclient::~tcpclient() {
  if (m_tcpClient) {
    delete m_tcpClient;
    m_tcpClient = nullptr;
  }
  delete ui;
}

void tcpclient::create_homeDir() {
  //创建应用文件夹

  QDir folder(PATH_ANDROID_BASE);
  qDebug() << "current dir : " << folder << endl;
  bool exist = folder.exists(PATH_ANDROID_APP_HOME);
  if (exist) {
    // QMessageBox::warning(this, tr("createDir"), tr("Dir is already
    // existed!"));
  } else {
    //创建文件夹
    bool ok = folder.mkdir("Client");
    if (ok)
      QMessageBox::warning(this, tr("CreateDir"),
                           tr("Create Home Dir success!"));
    else
      QMessageBox::warning(this, tr("CreateDir"), tr("Create Home Dir fail"));
  }
  // create sub dir
  QVector<QString> dirArray = {PATH_ANDROID_APP_MUSIC};
  for (auto dir : dirArray) {
    if (folder.exists(dir))
      continue;
    bool ok = folder.mkdir(dir);
    if (ok == false) {
      QMessageBox::critical(this, tr("CreateDir"),
                            tr("Create Dir %1 fail").arg(dir));
      break;
    }
  }
}

void tcpclient::ReadData() {
  qDebug() << tr("%1").arg(1) << endl;
  if (true == m_nIsINS) // if in read-INS status
  {
    //取指令
  qDebug() << tr("%1").arg(2) << endl;
    qint64 uLen = get_INS_length();
    if (uLen == -2) return; // read noting, do nothing
    if (uLen <= 0)
    {
        qDebug() << "[INS], get INS length errro" << endl;
        m_tcpClient->readAll(); // empty recv buf
        return;
    }
    QByteArray buf = m_tcpClient->read(uLen);
    if (buf.size() != uLen)
    {
        ui->plainTextEditRecv->appendPlainText("read INS errro");
        m_tcpClient->readAll(); // empty recv buf
        return;
    }
    qDebug() << "recv ins: " << buf << endl;
    QString qstrMsg_error = "";
    qint64 nType = codecodeSys::decode_type(buf, qstrMsg_error);
    qint64 uAct_name, uAct_obj, uAct_val;

    switch (nType)
    {
        case TYPE_ACT:
        {
            qDebug() << "ins: " << buf << endl;
            ui->plainTextEditRecv->appendPlainText(buf + '\n');
            qint64 dueTime = codecodeSys::decode_dueTime(buf, qstrMsg_error);
            if (dueTime == -1)
            {
                QMessageBox::critical(this, tr("INS error"), qstrMsg_error);
                m_tcpClient->readAll(); // empty recv buf
                SendData(qstrMsg_error);
                return;
            }

            //test
            ui->plainTextEditRecv->appendPlainText("dueTime: " + QString::number((dueTime)));
            ui->plainTextEditRecv->appendPlainText("current time: " + QString::number(QTime::currentTime().msecsSinceStartOfDay()));
            //test end

            codecodeSys::decode_act(buf, uAct_obj, uAct_name, uAct_val, qstrMsg_error);
            if (uAct_obj == ACT_OBJECT_PLAYER)
            {
                while (QTime::currentTime().msecsSinceStartOfDay() < dueTime)
                {
                    ui->plainTextEditRecv->appendPlainText(QString::number(QTime::currentTime().msecsSinceStartOfDay()) + " waiting~~");
                } // wait until due time
                emit evoke_music(buf);
            }
            else if (uAct_obj == ACT_OBJECT_HOMEPAGE)
            {
                emit evoker_homePage(buf);
            }

            break;
        }
        case TYPE_FILE:
        {
            qDebug() << "ins: " << buf << endl;
  qDebug() << tr("%1").arg(4) << endl;
            //初始化
            qint64 nRet = codecodeSys::decode_file(buf, m_mdiFile.fileName, m_mdiFile.fileSize, m_mdiFile.channelNumber, qstrMsg_error);
            if (nRet != 0) // error handle
            {
                QMessageBox::critical(this, tr("INS error"), qstrMsg_error);
                SendData(qstrMsg_error);
                return;
            }

            m_nIsINS = false;
            m_mdiFile.recvSize = 0;
            qDebug() << "file name = " << m_mdiFile.fileName << endl;
            qDebug() << "file size = " << m_mdiFile.fileSize << endl;
            //存储文件
            QString qstrFilePath = PATH_ANDROID_APP_MUSIC + m_mdiFile.fileName;
            ui->plainTextEditRecv->appendPlainText(
                tr("save path = %1").arg(qstrFilePath));
            m_mdiFile.file.setFileName(qstrFilePath);

            bool isOk = m_mdiFile.file.open(QIODevice::WriteOnly);
            if (!isOk)
            {
              qDebug() << "WriteOnly error";
              QMessageBox::critical(this, tr("error"),
                                    tr("can't write file, please check your access to your phone's file system and then try to receive again!"));

              m_tcpClient->disconnectFromHost(); //断开连接
              return; //如果打开文件失败，中断函数
            }
            //显示接收文件信息
            QString str = QString(tr("接收文件： [%1 : %2 kb]"))
                              .arg(m_mdiFile.fileName)
                              .arg(static_cast<double>(m_mdiFile.fileSize) / 1024);
            //QMessageBox::information(this, tr("file info"), str); // this funking stuff would block the main thread
            ui->plainTextEditRecv->appendPlainText(str);
            //设置进度条
            ui->progressBar->setMinimum(0);                         //最小值
            ui->progressBar->setMaximum(m_mdiFile.fileSize / 1024); //最大值
            ui->progressBar->setValue(0);                           //当前值
  qDebug() << tr("%1").arg(4.1) << endl;
  qDebug() << "m_nIsINS: " << m_nIsINS << endl;
            // test
//            buf = m_tcpClient->readAll();
//  qDebug() << "buf size: " << buf.size() << endl;
            // test
            break;
        }
        case TYPE_MSG:
        {
            qDebug() << "ins: " << buf << endl;
  qDebug() << tr("%1").arg(5) << endl;
            //初始化
            ui->plainTextEditRecv->appendPlainText(buf);
            break;
        }
        default:
        {
            qDebug() << "ins: " << buf << endl;
  qDebug() << tr("%1").arg(6) << endl;
            Reset_fileRecvStatus();
            SendData(qstrMsg_error);
            QMessageBox::critical(this, tr("INS error"), qstrMsg_error);
            return;
        }

    }
    // INS process end
  }
  else // recv file
  {
  qDebug() << tr("%1").arg(7) << endl;
        RecvFile();
  }
}

void tcpclient::RecvFile()
{
  qDebug() << tr("%1").arg(8) << endl;
    /* recv media file */
    QByteArray buf = m_tcpClient->read(m_mdiFile.fileSize - m_mdiFile.recvSize);
    qint64 nLen = m_mdiFile.file.write(buf);

    if (nLen > 0)
    {
      m_mdiFile.recvSize += nLen; //累积接收大小
      qDebug() << tr("current size = ") << nLen << endl;
      ui->plainTextEditRecv->appendPlainText(tr("current size = ") +
                                             tr("%1\n").arg(nLen));
    }
    //更新进度条
    ui->progressBar->setValue(m_mdiFile.recvSize / 1024);
    ui->plainTextEditRecv->appendPlainText(
        tr("recv size = %1\n").arg(m_mdiFile.recvSize));

    if (m_mdiFile.recvSize == m_mdiFile.fileSize)
    {
  qDebug() << tr("%1").arg(9) << endl;
      //发送接收文件完成的消息 respond
      m_tcpClient->write("recv finish");

      ui->plainTextEditRecv->appendPlainText(tr("文件%1接收完毕\n").arg(m_mdiFile.fileName));

      m_mdiFile.file.close(); //关闭文件

      /* reset status and wait for next transmission */
      Reset_fileRecvStatus();
      /* incase dismiss next INS */
      ReadData();
    }
}

void tcpclient::Reset_fileRecvStatus()
{
    m_nIsINS = true;
    ui->progressBar->setValue(0);
    m_mdiFile.reset_status();
}

qint64 tcpclient::get_INS_length()
{
    QByteArray prefix = m_tcpClient->read(6);
    if (prefix.isEmpty())
    {
        return -2;
    }
    if (prefix.back() != '#')
    {
        ui->plainTextEditRecv->appendPlainText("INS length errro");
        ui->plainTextEditRecv->appendPlainText(tr("prefix: %1").arg(prefix.constData()));
        QMessageBox::critical(this, tr("INS error"), tr("get INS length error"));
        return -1;
    }
    return prefix.mid(0, 5).toInt();
}

void tcpclient::ReadError(QAbstractSocket::SocketError) {

  /* 处理TCP连接错误 */
  m_tcpClient->disconnectFromHost(); // 先断开TCP连接
  ui->pushButtonDisconnect->setText(tr("连接"));
  m_isConnected = false;
  Reset_fileRecvStatus();

  QString msg_error(tr("failed to connect server because %1")
                     .arg(m_tcpClient->errorString())); // 弹出错误消息窗口
  QMessageBox::critical(this, tr("TCP error"), msg_error);
  qDebug() << "TCP error\n" << msg_error << endl;
}

void tcpclient::SendData(QString data) {
  /* 发送数据到服务器端 */
  if (data != "") {
    m_tcpClient->write(data.toLatin1());
  }
}

void tcpclient::ShowMyself() { this->show(); }

void tcpclient::on_pushButtonDisconnect_clicked() {
  /* 尝试连接服务器 */
  if (m_isConnected == false) {
    m_tcpClient->connectToHost(ui->lineEditIP->text(),
                               ui->lineEditPort->text().toInt());
    QString msg("trying to connect to the host %1 at port %2");
    qDebug() << msg.arg(ui->lineEditIP->text()).arg(ui->lineEditPort->text());

    /* 等待连接 */
    if (m_tcpClient->waitForConnected(10000)) { // connect successfully

      qDebug() << "connected!" << Qt::endl;
      ui->pushButtonDisconnect->setText(tr("断开"));
      ui->pushButtonDisconnect->setEnabled(true);
      m_isConnected = true;
    } else { // connect fail

      QMessageBox::critical(this, tr("connect error"),
                            tr("fail to connect to the server"));
    }
    return;
  }

  /* 断开与服务器连接 */
  m_tcpClient->disconnectFromHost();
  if (m_tcpClient->state() == QAbstractSocket::UnconnectedState or
      m_tcpClient->waitForDisconnected(
          10000)) // 如果处于未连接状态或者等待一段时间后成功断开连接
  {
    ui->pushButtonDisconnect->setText("连接");
  }
  m_isConnected = false;
  Reset_fileRecvStatus();
}

void tcpclient::on_pushButtonClearWindow_clicked() {
  /* 清除接收窗口 */
  ui->plainTextEditRecv->setReadOnly(false);
  ui->plainTextEditRecv->clear();

  ui->plainTextEditRecv->setReadOnly(true);
}

void tcpclient::on_pushButton_main_clicked() {
  hide();
  emit show_mainWindow();
}

void tcpclient::on_pushButton_tcpSetting_clicked() { ShowMyself(); }

void tcpclient::on_pushButton_music_clicked() {
  hide();
  emit show_musicPlayer();
}
