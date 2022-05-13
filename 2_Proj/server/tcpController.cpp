#include "tcpController.h"
#include "ui_tcpController.h"
using Qt::endl;

tcpController::tcpController(QMainWindow *parent)
    : QMainWindow(parent), ui(new Ui::tcpController) {
  /* 初始化服务器 */
  ui->setupUi(this);
  m_tcpServer = new QTcpServer(this);

  /* init player */
  m_player.setAudioOutput(&m_audioOutput);


  /* initiate variables */
  m_bIsListening = false;
  m_playMode = PLAYER_PLAY_MODE_LIST_CYCLE;

  /* 初始化配置 */
  setWindowTitle(tr("controller"));
  ui->lineEditIP->setText(QNetworkInterface()
                              .allAddresses()
                              .at(3)
                              .toString()); // 自动填充本机ip地址
  ui->pushButtonDisconnect->setText(tr("开始监听"));
  ui->pushButtonDisconnect->setEnabled(true);
  ui->plainTextEditRecv->setReadOnly(true);
  ui->pushButtonSend->setEnabled(false);
  ui->pushButton_sendFile->setEnabled(false);
  ui->pushButton_chooseFile->setEnabled(false);
//  ui->lineEditIP->setText("192.168.123.100");
  ui->lineEditPort->setText("30000");
  on_pushButtonDisconnect_clicked(); // default switch on listening
  m_musicDir.append(QDir("../../../3_Resource/MusicLibrary_raw/"));

  /* init widget */
  ui->volumeSlider->setVisible(false);
  ui->volumeSlider->setRange(0, 100);
  ui->volumeSlider->setTracking(false);
  ui->positionSlider->setTracking(false); // emit after relase slider
  m_syncBar = new RoundProgressBar(ui->groupBox_docker);
  m_syncBar->setOutterBarWidth(11);
  m_syncBar->setInnerBarWidth(9);
  m_syncBar->setControlFlags(RoundProgressBar::all);
  m_syncBar->hide();
//  m_loadBar = new RoundProgressBar(ui->groupBox_docker);
//  m_loadBar->setOutterBarWidth(11);
//  m_loadBar->setInnerBarWidth(9);
//  m_loadBar->setControlFlags(RoundProgressBar::all);
//  m_loadBar->hide();

  /* tcp connect */
  connect(m_tcpServer, &QTcpServer::newConnection, this,
          &tcpController::NewConnectionSlot);

  /* player connect */
  connect(ui->positionSlider, &QAbstractSlider::valueChanged, this, &tcpController::setPosition);
  decltype(QMediaPlayer::PlayingState) stateBefore = QMediaPlayer::PausedState; // status before pause the player
  connect(ui->positionSlider, &QAbstractSlider::sliderPressed, &m_player, [=, &stateBefore](){
      stateBefore = m_player.playbackState();
      m_player.pause();
  }); // pause playing while set pos
  connect(ui->positionSlider, &QAbstractSlider::sliderReleased, &m_player, [=, &stateBefore](){
      qDebug() << "before state = " << stateBefore << endl;
      if (stateBefore == QMediaPlayer::PlayingState)
      {
          delay_ms();
          m_player.play();
      }
  });
  connect(&m_player, &QMediaPlayer::positionChanged, this, &tcpController::updatePosition);
  connect(&m_player, &QMediaPlayer::durationChanged, this, &tcpController::updateDuration);
  connect(&m_player, &QMediaPlayer::playbackStateChanged, this, [=](QMediaPlayer::PlaybackState newState){
      qDebug() << "state changed" << endl;
     if (m_player.position() >= m_player.duration())
     {
         qDebug() << "cycle " << endl;
         switch (m_playMode)
         {
         case PLAYER_PLAY_MODE_LIST_CYCLE:
             on_btnNext_clicked();
             break;
         case PLAYER_PLAY_MODE_SINGLE_CYCLE:
             on_pushButton_rePlay_clicked();
             break;
         default:
             break;
         }
     }
     updatePlayBtnIcon();
  });
}

tcpController::~tcpController()
{
    delete ui;
    if (m_outFile.file.isOpen())
    {
        m_outFile.file.close();
    }
    if (m_syncBar)
    {
        delete m_syncBar;
        m_syncBar = nullptr;
    }
    if (m_loadBar)
    {
        delete m_loadBar;
        m_loadBar = nullptr;
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
  for (int i = 0; i < m_ListTcpClient.length(); ++i)
  {
    /*  recv data from client */
    QByteArray buffer = m_ListTcpClient[i]->readAll();
    if (buffer.isEmpty()) continue;

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
        m_hasSend++;
        m_syncBar->setValue(m_hasSend);
        if (m_hasSend >= m_totalSend)
        {
            qDebug() << "all files send successfully" << endl;
            m_totalSend = m_hasSend = 0;
            m_syncBar->hide();
        }
        qDebug() << tr("send file to device %1 success").arg(identityController::getInstance().whichId(m_ListTcpClient[i])) << endl;
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
    QByteArray ins = codecodeSys::INS_generator(data).toLatin1();
    qint64 nWriteSize = client->write(ins);
    if (nWriteSize == ins.length())
    {
        qDebug() << "send one INS " << ins <<  " success" << endl;
    }
    else
    {
        qDebug() << "send one INS " << ins << " fail" << endl;
    }
    return nWriteSize;
}

qint64 tcpController::sendData2all(const QByteArray &data)
{
    QElapsedTimer testTimer;
    testTimer.restart();
    decltype(m_ListTcpClient.size()) nSize = m_ListTcpClient.size();
    if (nSize == 0)
    {
        QMessageBox::warning(this, tr("send data"), tr("no client is connected!"));
        return -1;
    }
    qint64 uTotalWriteSize = 0;
    calculator::getInstance().startDelayTimer(); // 开始计算发送延时
    for (int i = 0 ; i < nSize ; ++ i)
    {
        uTotalWriteSize += sendData2single(m_ListTcpClient.at(i), data);
    }
    qDebug() << "local test timer = " << testTimer.elapsed() << endl;
    return uTotalWriteSize;
}

void tcpController::NewConnectionSlot() {
  m_currentClient = m_tcpServer->nextPendingConnection();
  /* add to client list and allocate an id */
  m_ListTcpClient.append(m_currentClient); // add to list

  assignId(); // assign id to client

  ui->comboBox->addItem(
      tr("%1:%2")
          .arg(m_currentClient->peerAddress().toString().split("::ffff:")[1])
          .arg(m_currentClient->peerPort()));

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
  QString ins = (INS_MSG.arg(qsSend));
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
      sendData2single(m_ListTcpClient[i], ins.toUtf8());
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
        (INS_FILE.arg(m_outFile.fileName).arg(m_outFile.fileSize).arg(m_outFile.channelNumber));
    //轮流发送file instruction
    qint64 len = sendData2all(ins.toUtf8());
    if (len > 0) // send successfully
    {
      sendFile();
    }
    else
    {
        QMessageBox::critical(this, __FUNCTION__ ,("send fail!"));
    }
}

void tcpController::delay_ms()
{
    calculator &cal = calculator::getInstance();
    qint64 ms = cal.playDelay_server(cal.getDelayTime());
    QTime dieTime = QTime::currentTime().addMSecs(ms);
         while( QTime::currentTime() < dieTime )
             QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
         qDebug() << "server delay = " << ms << endl;
}

void tcpController::delay_ms(qint64 ms)
{
    QTime dieTime = QTime::currentTime().addMSecs(ms);
         while( QTime::currentTime() < dieTime )
             QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
         qDebug() << "server delay = " << ms << endl;
}

void tcpController::updatePosition(qint64 pos)
{
    ui->positionSlider->setValue(pos);
    ui->Label_position->setText(forMatTime(pos));
    ui->Label_position_2->setText(forMatTime(m_player.duration()));
}

void tcpController::updateDuration(qint64 duration)
{
    ui->positionSlider->setRange(0, static_cast<int>(duration));
    ui->positionSlider->setEnabled(static_cast<int>(duration) > 0);
    ui->positionSlider->setPageStep(static_cast<int>(duration));
    ui->Label_position_2->setText(forMatTime(m_player.duration()));
}

void tcpController::updatePlayBtnIcon()
{
    if (m_player.playbackState() == QMediaPlayer::PausedState)
    {
        ui->btnPlay->setIcon(QIcon(":/image/image/image/24gl-play.png"));
    }
    else
    {
        ui->btnPlay->setIcon(QIcon(":/image/image/image/24gl-pause.png"));
    }
}

QString tcpController::forMatTime(qint64 timeMilliSeconds)
{
    qint64 seconds = timeMilliSeconds / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}




void tcpController::on_pushButton_PlayList_clicked()
{
    if (m_player.playbackState() == QMediaPlayer::PlayingState)
    {
        QMessageBox::warning(this, tr(__FUNCTION__), tr("please pause music first then sync the music library"));
        return;
    }
    QStringList filters;
    m_musicInfoList.clear();
    filters << QString("*.mp3");

    ui->ListWidget_musicName->clear();
    qint64 nCnt = 0; // number of music file

    for (auto path: m_musicDir)
    {
        /* check illegal chara */
        QDirIterator dir_iterator_check(path.path(), filters, QDir::Files | QDir::NoSymLinks);
        while (dir_iterator_check.hasNext())
        {
            nCnt ++;
            dir_iterator_check.next();
            /* 如果文件名中出现了#或者_字符或空格强制改为字符~ */
            QDir dir(dir_iterator_check.path());
            QString qstrFileName = dir_iterator_check.fileName();
            if (qstrFileName.contains('#') or qstrFileName.contains('_') or qstrFileName.contains(' '))
            {
                qstrFileName.replace('#', '~');
                qstrFileName.replace('_', '~');
                qstrFileName.replace(' ', '~');
                bool ok = dir.rename(dir_iterator_check.filePath(), dir_iterator_check.path() + '/' + qstrFileName);
                if (!ok)
                {
                    QMessageBox::critical(this, tr(__FUNCTION__), tr("contain illegal characters, rename fail, please try to manual rename"));
                    return;
                }
            }
        }
        /* read music info */
        qDebug() << "nCnt = " << nCnt << endl;
//        m_loadBar->setRange(0, nCnt);
        nCnt = 0;
//        m_loadBar->setValue(nCnt);
//        m_loadBar->show();

        QDirIterator dir_iterator(path.path(), filters, QDir::Files | QDir::NoSymLinks);
        while (dir_iterator.hasNext())
        {
            dir_iterator.next();
            m_musicInfoList.append(dir_iterator.fileInfo());
            qDebug()<<"name:"<<dir_iterator.fileInfo().fileName()<<"path:"<<dir_iterator.fileInfo().filePath();
            qint64 nChannels = -1;
            if (m_musicChannels.find(dir_iterator.fileInfo().fileName()) == m_musicChannels.end())
            {
                emit evoke_getChannels(dir_iterator.fileInfo().absoluteFilePath(), nChannels);
                if (nChannels == -1)
                {
                    qDebug() << "get channels error" << endl;
                }
                m_musicChannels[dir_iterator.fileInfo().fileName()] = nChannels;
            }
            else
            {
                nChannels = m_musicChannels[dir_iterator.fileInfo().fileName()];
            }
//            m_loadBar->setValue(nCnt++);
            ui->ListWidget_musicName->addItem(dir_iterator.fileName().section('.', 0, 0) + tr("  %1 声道").arg(nChannels));
        }
    }
//    m_loadBar->hide();

    m_totalSend = 0;
    qint64 nClientNumber = m_ListTcpClient.size();
    if (!m_musicInfoList.empty()) m_curSelectSong = m_musicInfoList.at(0); // select first one by default
    for (auto music: m_musicInfoList)
    {
        qint64 nChannels = m_musicChannels[music.fileName()];
        if ( nChannels == 2)
        {
            emit evoke_split2(music.absoluteFilePath());
            m_totalSend += nClientNumber * 2;
        }
        else if (nChannels == 6)
        {
            emit evoke_split6(music.absoluteFilePath());
            m_totalSend += nClientNumber * 6;
        }
        else
        {
            qDebug() << __FUNCTION__ << "file name : " << music.fileName() << endl << "invalid channels : " << nChannels << endl;
        }
    }
    m_syncBar->setRange(0, m_totalSend);
    m_syncBar->setValue(0);
    m_syncBar->show();
}

void tcpController::on_ListWidget_musicName_doubleClicked(const QModelIndex &index)
{
    m_curSelectSong = m_musicInfoList.at(index.row()); // set current song

    setPlayerSource();
    on_btnPlay_clicked();
    on_btnPlay_clicked();
    setPosition_force(0);
}

void tcpController::setChannel(qint64 id, qint64 channelNumber)
{
    QString ins;
    calculator::getInstance().startDelayTimer(); // casually give a delay
    ins = (codecodeSys::code_act(ACT_OBJECT_PLAYER, ACT_NAME_ASSIGN_CHANNEL, channelNumber));

    sendData2single(identityController::getInstance().whichClient(id), ins.toUtf8());
}

void tcpController::setPosition(int pos)
{
    if (qAbs(m_player.position() - pos) >= 100)
    {
        QString ins;
        ins = (codecodeSys::code_act(ACT_OBJECT_PLAYER, ACT_NAME_SET_POS, pos));
        sendData2all(ins.toUtf8());

        m_player.setPosition(pos);
    }
}

void tcpController::setPosition_force(int pos)
{
    qDebug() << "force setPositino" << endl;
    QString ins;
    ins = (codecodeSys::code_act(ACT_OBJECT_PLAYER, ACT_NAME_SET_POS, pos));
    sendData2all(ins.toUtf8());

    m_player.setPosition(pos);
}

void tcpController::play_force()
{
    QString ins;
    ins = (INS_PLAY);
    sendData2all(ins.toUtf8());

    delay_ms();
    m_player.play();
}

void tcpController::pause_force()
{
    QString ins;
    ins = (INS_PAUSE);
    sendData2all(ins.toUtf8());

    delay_ms();
    m_player.play();
}

bool tcpController::setPlayerSource()
{
    QString ins;
//    QString fileName = ui->ListWidget_musicName->currentIndex().data().toString();
    QString fileName = m_musicInfoList[ui->ListWidget_musicName->currentRow()].fileName().section('.', 0, 0);
    qDebug() << "set source file name: " << fileName << endl;
    ins = (codecodeSys::code_act(ACT_OBJECT_PLAYER, ACT_NAME_SET_SOURCE, fileName));
    sendData2all(ins.toUtf8());

    delay_ms();
    m_player.setSource(QUrl::fromLocalFile(m_curSelectSong.absoluteFilePath()));
    return (m_player.source().toString() != "");
}

bool tcpController::setPlayerSource(QString abPath)
{
    m_player.setSource(QUrl::fromLocalFile(abPath));
    return (m_player.source().toString() != "");
}

qint64 tcpController::assignId()
{
    qint64 uId = identityController::getInstance().id_allocateId(m_currentClient); // allocate id
    emit evoke_homePage_addItem(uId);
    calculator::getInstance().startDelayTimer(); // casually give a delay
    QString ins = (INS_ASSIGN_ID.arg(uId));
    sendData2single(m_currentClient, ins.toUtf8()); // send id to client
    return 0;
}

void tcpController::on_pushButton_rePlay_clicked()
{
    if (m_musicInfoList.empty())
    {
        return;
    }

    /* remote */
    QString ins;
    ins = (codecodeSys::code_act(ACT_OBJECT_PLAYER, ACT_NAME_REPLAY, 1));
    sendData2all(ins.toUtf8());

    /* self */
    delay_ms();
    m_player.stop();
    m_player.play();
}

void tcpController::on_btnPlay_clicked()
{
    if (m_musicInfoList.empty())
    {
        return;
    }
    QString ins;
    if (m_player.playbackState() == QMediaPlayer::PausedState)
    {
        ins = (INS_PLAY);
        sendData2all(ins.toUtf8());

        delay_ms();
        m_player.play();
    }
    else
    {
        ins = (INS_PAUSE);
        sendData2all(ins.toUtf8());

        delay_ms();
        m_player.pause();
    }
}

void tcpController::on_btnPre_clicked()
{
    if (m_musicInfoList.empty())
    {
        return;
    }
    qint64 nCurRow = ui->ListWidget_musicName->currentRow();
    qint64 nSize = ui->ListWidget_musicName->count();
    nCurRow = (nCurRow - 1 + nSize) % nSize;
    qDebug() << "cur row = " << nCurRow <<endl;
    ui->ListWidget_musicName->setCurrentRow(nCurRow);
    m_curSelectSong = m_musicInfoList.at(nCurRow);

    setPlayerSource();
    on_btnPlay_clicked();
    on_btnPlay_clicked();
    setPosition_force(0);
}


void tcpController::on_btnNext_clicked()
{
    if (m_musicInfoList.empty())
    {
        return;
    }
    qint64 nCurRow = ui->ListWidget_musicName->currentRow();
    qint64 nSize = ui->ListWidget_musicName->count();
    nCurRow = (nCurRow + 1) % nSize;
    qDebug() << "cur row = " << nCurRow <<endl;
    ui->ListWidget_musicName->setCurrentRow(nCurRow);
    m_curSelectSong = m_musicInfoList.at(nCurRow);

    setPlayerSource();
    on_btnPlay_clicked();
    on_btnPlay_clicked();
    setPosition_force(0);
}


void tcpController::on_volumeSlider_valueChanged(int value)
{
    QString ins;
    ins = (codecodeSys::code_act(ACT_OBJECT_PLAYER, ACT_NAME_SET_VOLUME, value));
    sendData2all(ins.toUtf8());

    delay_ms();
    m_audioOutput.setVolume(static_cast<double>(value) / 100.0);
}


void tcpController::on_btnVolume_clicked()
{
     if(ui->volumeSlider->isHidden()){
        ui->volumeSlider->show();
    }else{
        ui->volumeSlider->hide();
    }
}


void tcpController::on_btnPlayMode_clicked()
{
    qint64 nTotal = 2;
    m_playMode = (m_playMode + 1) % nTotal;
    if (m_playMode == PLAYER_PLAY_MODE_LIST_CYCLE)
    {
        ui->btnPlayMode->setIcon(QIcon(":/image/image/image/24gl-repeat.png"));
    }
    else if (m_playMode == PLAYER_PLAY_MODE_SINGLE_CYCLE)
    {
        ui->btnPlayMode->setIcon(QIcon(":/image/image/image/24gl-repeatOnce.png"));
    }
}
