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
  m_isStart = true;

  /* initiate widget */
  ui->progressBar->setValue(0);             // set progress bar initial value
  ui->plainTextEditRecv->setReadOnly(true); // 接收窗口设为只读
  ui->pushButtonDisconnect->setText(tr("连接"));
  ui->lineEditIP->setText("192.168.123.100");
  ui->lineEditPort->setText("30000");

  /* 连接信号与槽 */
  connect(m_tcpClient, SIGNAL(readyRead()), this,
          SLOT(ReadData())); // 连接准备读就绪信号与读数据槽函数
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

void tcpclient::save_file()
{
    //创建应用文件夹
    QDir *folder = new QDir;
    bool exist = folder->exists(PATH_ANDROID_APP_MUSIC);
    if(exist)
    {
        //QMessageBox::warning(this, tr("createDir"), tr("Dir is already existed!"));
    }
    else
    {
        //创建文件夹
        bool ok = folder->mkdir(PATH_ANDROID_APP_MUSIC);
        if(ok)
            QMessageBox::warning(this, tr("CreateDir"), tr("Create Dir success!"));
        else
            QMessageBox::warning(this, tr("CreateDir"), tr("Create Dir fail"));
    }
    //存储文件
    QString qstrFilePath = PATH_ANDROID_APP_MUSIC + m_mdiFile.fileName + ".mp3";
    QFile file(qstrFilePath);
    qDebug() << "save file path : " << qstrFilePath << endl;
    if(!file.open(QIODevice::ReadWrite | QIODevice::))
    {
       QMessageBox::warning(this,tr("错误"),tr("打开文件失败，数据保存失败"));
       return;
    }
    else
    {
       if(!file.isReadable())
       {
           QMessageBox::warning(this,tr("错误"),tr("该文件不可读，数据保存失败"));
       }
       else
       {
           QTextStream out(&file);
           for(int i=0;i<dateline;i++){
               alltext+=mostlist[i].part[0]+"  "+mostlist[i].part[1]+"\n";
           }
           out<<alltext<<endl<<QObject::tr("\n date end");
           QMessageBox::warning(this,tr("信息"),tr("信息存储成功！\n可在系统根目录文件'UIDdate'文件夹中查看！"));
       }
    }
    file.close();
}

void tcpclient::ReadData() {
  /* recv media file */
  //取出接收的内容
    static int cnt = 0;
  QByteArray buf = m_tcpClient->readAll();

  if (true == m_isStart) { // recv file header
    cnt = 1;
    m_isStart = false;

    //            分段   0   0  1  1  2    2
    //解析头部信息 buf = "hello##1024##333333"
    //            QString str = "hello##1024";
    //            str.section("##",0,0);//这个拆出来hello，其中##是分段标识符，格式为：段1+标识符+段2+标识符+段3+标识符+……

    //初始化
    m_mdiFile.fileName = QString(buf).section("##", 0, 0);
    m_mdiFile.fileSize = QString(buf).section("##", 1, 1).toInt();
    m_mdiFile.recvSize = 0;
    qDebug() << "file name = " << m_mdiFile.fileName << endl;
    qDebug() << "file size = " << m_mdiFile.fileSize << endl;
   //打开文件
    m_mdiFile.file.setFileName(m_mdiFile.fileName);

    bool isOk = m_mdiFile.file.open(QIODevice::WriteOnly);
    if (false == isOk) {
      qDebug() << "WriteOnly error 38 ";
      QMessageBox::critical(this, tr("error"),
                            tr("can't write file, please check your access to "
                               "file system and then try to connect again!"));

      m_tcpClient->disconnectFromHost(); //断开连接
      m_tcpClient->close();              //关闭套接字
      m_isConnected = false;             // set status

      return; //如果打开文件失败，中断函数
    }
    //弹出对话框，显示接收文件信息
    ui->plainTextEditRecv->appendPlainText(
        tr("recv FileName: %1\nrecv FileSize: %2\n")
            .arg(m_mdiFile.fileName, m_mdiFile.fileSize));

    QString str = QString(tr("接收文件： [%1 : %2 kb]"))
                      .arg(m_mdiFile.fileName)
                      .arg(static_cast<double>(m_mdiFile.fileSize) / 1024);
    QMessageBox::information(this, tr("文件信息"), str);

    //设置进度条
    ui->progressBar->setMinimum(0);                         //最小值
    ui->progressBar->setMaximum(m_mdiFile.fileSize / 1024); //最大值
    ui->progressBar->setValue(0);                           //当前值

  } else // recv file
  {
    cnt++;
    qint64 nLen = m_mdiFile.file.write(buf);

    if (nLen > 0) {
      m_mdiFile.recvSize += nLen; //累积接收大小
      qDebug() << tr("current size = ") << nLen << endl;
      ui->plainTextEditRecv->appendPlainText(tr("current size = ") +
                                             tr("%1\n").arg(nLen));
    }
    //更新进度条
    ui->progressBar->setValue(m_mdiFile.recvSize / 1024);
    ui->plainTextEditRecv->appendPlainText(tr("recv size = %1\n").arg(m_mdiFile.recvSize));

    if (m_mdiFile.recvSize == m_mdiFile.fileSize) {
      //先给服务发送（接收文件完成的消息）
      m_tcpClient->write("recv finish");

      QMessageBox::information(this, tr("完成"), tr("文件接收完毕"));

      m_mdiFile.file.close(); //关闭文件

      /* reset status and wait for next transmission */
      m_isStart = true;
    }
  }
  qDebug() << tr("evoke %1 times").arg(cnt)<< endl;
}

void tcpclient::ReadError(QAbstractSocket::SocketError) {

  /* 处理TCP连接错误 */
  m_tcpClient->disconnectFromHost(); // 先断开TCP连接
  ui->pushButtonDisconnect->setText(tr("连接"));
  m_isConnected = false;
  m_isStart = true;

  QMessageBox msgBox;
  msgBox.setText(tr("failed to connect server because %1")
                     .arg(m_tcpClient->errorString())); // 弹出错误消息窗口
}

void tcpclient::SendData(QString data) {
  /* 发送数据到服务器端 */
  if (data != "") {
    m_tcpClient->write(data.toLatin1());
  }
}

void tcpclient::ShowMyself()
{
  this->show();
}

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
  m_isStart = true;
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

void tcpclient::on_pushButton_tcpSetting_clicked() {
    ShowMyself();
}

void tcpclient::on_pushButton_music_clicked() {
    hide();
    emit show_musicPlayer();
}
