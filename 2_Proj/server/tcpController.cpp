#include "tcpController.h"
#include "ui_tcpController.h"
using Qt::endl;

tcpController::tcpController(QMainWindow *parent)
    : QMainWindow(parent), ui(new Ui::tcpController) {
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
  on_pushButtonDisconnect_clicked(); // default switch on listening
  m_musicDir.append(QDir("D:/Dev/CourseDesign/MultiStereo/3_Resource/MusicLibrary_raw/"));

  /* connect */
  connect(m_tcpServer, &QTcpServer::newConnection, this,
          &tcpController::NewConnectionSlot);

}

tcpController::~tcpController()
{
    delete ui;
    if (m_outFile.file.isOpen())
    {
        m_outFile.file.close();
    }
}

void tcpController::sendFile() {
  qDebug() << tr("send file begin") << endl;
  qint64 len = 0;
  do {
    //每次发送数据的大小 up to 4KB
    char buf[4 * 1024] = {0};
    len = 0;
    qint64 totalSend = 0;
    //往文件中读数据
    len = m_outFile.file.read(buf, sizeof(buf));
//    qDebug() << tr("read data len = ") << len << endl;
    //发送数据，读多少 ，发多少 * number of client
    for (auto client : m_ListTcpClient) {
      totalSend += client->write(buf, len);
    }
//    qDebug() << tr("total send data len = ") << totalSend << endl;
    //发送的数据需要积累
    m_outFile.sendSize += totalSend;
  } while (len > 0);
    qDebug() << tr("total send data len = ") << m_outFile.sendSize << endl;
  //是否发送文件完毕
  if (m_outFile.sendSize == m_outFile.fileSize * m_ListTcpClient.size()) {
    ui->plainTextEditSend->appendPlainText("文件all发送完毕");
  }
  else{
    ui->plainTextEditSend->appendPlainText("文件发送fail");
    QMessageBox::critical(this, tr("fatal"), tr("send file not complete!"));
  }
  /* end task */
  m_outFile.sendSize = 0;
  m_outFile.file.close(); // reset file status
  m_outFile.file.open(QIODevice::ReadOnly);
}

void tcpController::synchronize_musicFile(const QString &folderName, qint64 channelNumber)
{
    QDir dir(m_MusicLibBasePath + folderName);
    if (dir.exists() == false)
    {
        QMessageBox::critical(this, __FUNCTION__, tr("dir not exist"));
        return ;
    }
    QDirIterator dir_iterator(dir.path(), QDir::NoDotAndDotDot | QDir::AllEntries);
    while (dir_iterator.hasNext())
    {
        dir_iterator.next();
        QFileInfo info(dir_iterator.fileInfo());
        m_outFile.reset_status();
        m_outFile.filePath = info.absoluteFilePath();
        m_outFile.fileName = info.fileName();
        m_outFile.fileSize = info.size();
        m_outFile.file.setFileName(m_outFile.filePath);
        m_outFile.channelNumber = channelNumber;

        if(m_outFile.file.isOpen()) m_outFile.file.close();
        bool isOk = m_outFile.file.open(QIODevice::ReadOnly);
        if (false == isOk)
        {
            qDebug() << "只读方式打开文件失败";
            QMessageBox::critical(this, tr("error"), tr("open file fail! please check access right to selected file!"));
            break;
        }
        on_pushButton_sendFile_clicked();
    }
}

void tcpController::ReadData() {
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
    }
    IP_Port_Pre = IP_Port;
  }
}
/*
 *in:
 *  client(QTcpSocket*): the target client to send data
 *  data(QByteArray): the data to be sent
 *  len(qint64): at most len bytes of data from data to the device
 *
 * return:
 *  the size of data that have been actually successfully writen
 *  return -1 when fail
 */
qint64 tcpController::sendData2single(QTcpSocket *client, const QByteArray &data)
{
    qint64 nWriteSize = client->write(data);
    if (nWriteSize == data.length())
    {
        qDebug() << "send one INS success" << endl;
    }
    else
    {
        qDebug() << "send one INS fail" << endl;
    }
    return nWriteSize;
}

qint64 tcpController::sendData2all(const QByteArray &data)
{
    decltype(m_ListTcpClient.size()) nSize = m_ListTcpClient.size();
    if (nSize == 0)
    {
        QMessageBox::warning(this, tr("send data"), tr("no client is connected!"));
        return -1;
    }
    qint64 uTotalWriteSize = 0;
    for (int i = 0 ; i < nSize ; ++ i)
    {
        uTotalWriteSize += sendData2single(m_ListTcpClient.at(i), data);
    }
    if (data.size() * nSize != uTotalWriteSize)
    {
        qDebug() << "INS send fail" << endl;
    }
    else
    {
        qDebug() << "INS send success" << endl;
    }
    return uTotalWriteSize;
//    QtConcurrent::run(this,&tcpController::test); // why error?
}

void tcpController::NewConnectionSlot() {
  m_currentClient = m_tcpServer->nextPendingConnection();
  /* add to client list and allocate an id */
  m_ListTcpClient.append(m_currentClient); // add to list
  qint64 uId = identityController::getInstance().id_allocateId(m_currentClient); // allocate id
  emit evoke_homePage_addItem(uId);
  QString ins = codecodeSys::INS_generator(INS_ASSIGN_ID.arg(uId));
  sendData2single(m_currentClient, ins.toLatin1()); // send id to client

  ui->comboBox->addItem(
      tr("%1:%2")
          .arg(m_currentClient->peerAddress().toString().split("::ffff:")[1])
          .arg(m_currentClient->peerPort()));

  codecodeSys::setClientNumbers(m_ListTcpClient.length()); // update client number

  /* enable only if connexion is established*/
  ui->pushButtonSend->setEnabled(true);
  ui->pushButton_chooseFile->setEnabled(true);
  ui->pushButton_sendFile->setEnabled(true);

  /* attention! this connect process must be done after there indeed is a client(QtcpSocket) */
  connect(m_currentClient, &QIODevice::readyRead, this, &tcpController::ReadData);
  connect(m_currentClient, &QAbstractSocket::disconnected, this,
          &tcpController::disConnectedSlot);
}

void tcpController::disConnectedSlot() {
  /* 由于disconnected信号没有提供请求断开连接窗口的描述信息所以需要遍历定位再断开连接并删除
   */
  for (int i = 0; i < m_ListTcpClient.length(); ++i) {
    if (m_ListTcpClient[i]->state() == QAbstractSocket::UnconnectedState) {
      ui->comboBox->removeItem(ui->comboBox->findText(
          tr("%1:%2")
              .arg(m_ListTcpClient[i]->peerAddress().toString().split(
                  "::ffff:")[1])
              .arg(m_ListTcpClient[i]->peerPort())));
      emit evoke_homePage_RemoveItem(identityController::getInstance().whichId(m_ListTcpClient[i]));
      identityController::getInstance().id_recycleId(m_ListTcpClient[i]);
      m_ListTcpClient[i]->destroyed();
      m_ListTcpClient.removeAt(i);
    }
  }
  if (m_ListTcpClient.empty()) {
    ui->pushButtonSend->setEnabled(false);
  }

  codecodeSys::setClientNumbers(m_ListTcpClient.length()); // update
}

void tcpController::on_pushButtonDisconnect_clicked()
{
  if (m_bIsListening == false)
  {
     /* 开始监听端口连接请求 */
     bool ok = m_tcpServer->listen(QHostAddress::Any,
                                   ui->lineEditPort->text().toInt());
     if (ok)
     {
         ui->pushButtonDisconnect->setText("断开");

         /* 开始监听成功 */
         m_bIsListening = true;
     }
     else
     {
         QMessageBox::critical(this, tr("Server error"),
                             tr("fail to listening to port %1")
                                 .arg(ui->lineEditPort->text().toInt()));
     }
  }
  else
  {

    /* 关闭所有连接并停止监听窗口 */
    // 有问题，会崩溃：只保留disconnectFromtcpController其他删掉
    for (int i = 0; i < m_ListTcpClient.size(); ++i)
    {
        m_ListTcpClient.at(i)->disconnectFromHost();
        qDebug() << tr("size of list = %1").arg(m_ListTcpClient.size()) << endl;
    }
    m_ListTcpClient.clear();
    m_tcpServer->close();

    m_bIsListening = false;
    ui->pushButtonSend->setEnabled(false);
    ui->pushButtonDisconnect->setText(tr("开始监听"));
  }
}

void tcpController::on_pushButtonClearWindow_clicked() {
  ui->plainTextEditRecv->setReadOnly(false);
  ui->plainTextEditRecv->clear();
  ui->plainTextEditRecv->setReadOnly(true);
}

void tcpController::on_pushButtonSend_clicked() {
  QString qsSend = ui->plainTextEditSend->toPlainText();
  QString ins = codecodeSys::INS_generator(INS_MSG.arg(qsSend));
  if (ui->comboBox->count() == 0) // 如果发送窗口没有消息，或者没有已经建立的连接
  {
    QMessageBox::critical(
        this, tr("Error"),
        tr("no connection avaliable"));
    return;
  }

  // send msg to a specified client
  QString clientIP = ui->comboBox->currentText().split(":")[0];
  qDebug() << "clienIP :" << clientIP << endl;
  int clientPort = ui->comboBox->currentText().split(":")[1].toInt();
  qDebug() << "clienPort :" << clientPort << endl;

  for (int i = 0; i < m_ListTcpClient.length(); ++i)
  {
    qDebug() << m_ListTcpClient[i]->peerAddress().toString().split("::ffff:")[1] << endl;

    if (m_ListTcpClient[i]->peerAddress().toString().split("::ffff:")[1] == clientIP and m_ListTcpClient[i]->peerPort() == clientPort)
    {
      m_ListTcpClient[i]->write(ins.toLatin1());
      return;
    }
  }

}

void tcpController::on_pushButton_chooseFile_clicked() {
  m_outFile.filePath = QFileDialog::getOpenFileName(this, "open", "../");

  if (false == m_outFile.filePath.isEmpty()) {
    m_outFile.fileName.clear();
    m_outFile.fileSize = 0;

    //获取文件信息
    QFileInfo info(m_outFile.filePath);
    m_outFile.fileName = info.fileName(); //获取文件名字
    m_outFile.fileSize = info.size();     //获取文件大小
    qDebug() << tr("file size = ") << m_outFile.fileSize / 1024 << tr("kb") << endl;

    m_outFile.sendSize = 0; // size of already send part of this file

    //只读方式打开文件
    //指定文件的名字
    m_outFile.file.setFileName(m_outFile.filePath);

    //打开文件
    if (m_outFile.file.isOpen()) m_outFile.file.close();
    bool isOk = m_outFile.file.open(QIODevice::ReadOnly);
    if (false == isOk) {
      qDebug() << "只读方式打开文件失败";
      QMessageBox::critical(this, tr("error"), tr("open file fail! please check access right to selected file!"));
      return;
    }

    //提示打开文件路径
    ui->lineEdit_FileName->setText(m_outFile.filePath);

    ui->pushButton_sendFile->setEnabled(true);
  } else {
    qDebug() << "选择文件路径出错";
  }
}

void tcpController::on_pushButton_sendFile_clicked() {
    if (m_outFile.fileName.isEmpty())
    {
        QMessageBox::warning(this, tr("transmission"), tr("no file selected"));
        return;
    }
    // send instruction first
    QString ins =
        codecodeSys::INS_generator(INS_FILE.arg(m_outFile.fileName).arg(m_outFile.fileSize).arg(m_outFile.channelNumber));
      qDebug() << "INS : " << ins << endl;
    //轮流发送file instruction
    qint64 len = sendData2all(ins.toLatin1());
    if (len > 0) // send successfully
    {
      sendFile();
    }
    else
    {
        QMessageBox::critical(this, __FUNCTION__ ,("send fail!"));
    }
}

void tcpController::on_btnPlay_clicked()
{
    QString ins;
    static bool player_status = false;
    if (player_status == false)
    {
        ins = codecodeSys::INS_generator(INS_PLAY);
        sendData2all(ins.toLatin1());
        player_status = true;
    }
    else
    {
        ins = codecodeSys::INS_generator(INS_PAUSE);
        sendData2all(ins.toLatin1());
        player_status = false;
    }
}


void tcpController::on_pushButton_PlayList_clicked()
{
    QStringList filters;
    m_musicInfo.clear();
    filters << QString("*.mp3");

    ui->ListWidget_musicName->clear();

    for (auto path: m_musicDir)
    {
        /* check illegal chara */
        QDirIterator dir_iterator_check(path.path(), filters, QDir::Files | QDir::NoSymLinks,QDirIterator::Subdirectories);
        while (dir_iterator_check.hasNext())
        {
            dir_iterator_check.next();
            /* 如果文件名中出现了#或者_字符强制改为字符~ */
            QDir dir(dir_iterator_check.path());
            QString qstrFileName = dir_iterator_check.fileName();
            if (qstrFileName.contains('#') or qstrFileName.contains('_'))
            {
                qstrFileName.replace('#', '~');
                qstrFileName.replace('_', '~');
                bool ok = dir.rename(dir_iterator_check.filePath(), dir_iterator_check.path() + '/' + qstrFileName);
                if (!ok)
                {
                    QMessageBox::critical(this, tr(__FUNCTION__), tr("contain illegal characters, rename fail, please try to manual rename"));
                    return;
                }
            }
        }
        /* read music info */
        QDirIterator dir_iterator(path.path(), filters, QDir::Files | QDir::NoSymLinks,QDirIterator::Subdirectories);
        while (dir_iterator.hasNext())
        {
            dir_iterator.next();
            m_musicInfo.append(dir_iterator.fileInfo());
            qDebug()<<"name:"<<dir_iterator.fileInfo().fileName()<<"path:"<<dir_iterator.fileInfo().filePath();
            ui->ListWidget_musicName->addItem(dir_iterator.fileName());
        }
    }
    for (auto music: m_musicInfo)
    {
        emit evoke_split(music.absoluteFilePath());
    }
}

void tcpController::on_ListWidget_musicName_doubleClicked(const QModelIndex &index)
{

}

void tcpController::setChannel(qint64 id, qint64 channelNumber)
{
    QString ins;
    ins = codecodeSys::INS_generator(codecodeSys::code_act(ACT_OBJECT_PLAYER, ACT_NAME_ASSIGN_ID, channelNumber));
    sendData2single(identityController::getInstance().whichClient(id), ins.toLatin1());
}


void tcpController::on_pushButton_rePlay_clicked()
{
     QString ins;
     ins = codecodeSys::INS_generator(codecodeSys::code_act(ACT_OBJECT_PLAYER, ACT_NAME_REPLAY));
     sendData2all(ins.toLatin1());
}

