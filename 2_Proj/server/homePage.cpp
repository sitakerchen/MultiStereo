#include "homePage.h"
#include "ui_homePage.h"
using Qt::endl; using PC = ProcessControl;

homePage::homePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::homePage)
{
    ui->setupUi(this);
    this->setWindowTitle("Main Window");
    /* set currentDir to application Dir(binary executable file) */
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    PC::getInstance().setWorkingPath(); // set terminal/cmd's current path

    /* create tcpController and mediaProcess*/
    m_tcpCtr = new tcpController;
    m_MedaProces = new rawMediaProcess;

    /* initiate variables */
    m_boxList = {
                ui->comboBox_2left,
                ui->comboBox_2right,
                ui->comboBox_5left,
                ui->comboBox_5leftBac,
                ui->comboBox_5leftFor,
                ui->comboBox_5right,
                ui->comboBox_5rightBac,
                ui->comboBox_5rightFor
              };
    m_toChannelIndex[FILE_CHANNEL_NAME_2_LEFT] = FILE_CHANNEL_2_LEFT;
    m_toChannelIndex[FILE_CHANNEL_NAME_2_RIGHT] = FILE_CHANNEL_2_RIGHT;
    m_toChannelIndex[FILE_CHANNEL_NAME_5_LEFT_FORE] = FILE_CHANNEL_5_LEFT_FORE;
    m_toChannelIndex[FILE_CHANNEL_NAME_5_LEFT] = FILE_CHANNEL_5_LEFT;
    m_toChannelIndex[FILE_CHANNEL_NAME_5_LEFT_BACK] = FILE_CHANNEL_5_LEFT_BACK;
    m_toChannelIndex[FILE_CHANNEL_NAME_5_RIGHT_FORE] = FILE_CHANNEL_5_RIGHT_FORE;
    m_toChannelIndex[FILE_CHANNEL_NAME_5_RIGHT] = FILE_CHANNEL_5_RIGHT;
    m_toChannelIndex[FILE_CHANNEL_NAME_5_RIGHT_BACK] = FILE_CHANNEL_5_RIGHT_BACK;
    m_maxSize = 6;
    m_lastId = QVector<qint64>(m_maxSize, -1);

    /* initiate widget */
    addItem2AllChannelComboBox("null");

    /* connect */
    connect(m_tcpCtr, &tcpController::evoke_split2, m_MedaProces, &rawMediaProcess::split_2);
    connect(m_MedaProces, &rawMediaProcess::evoke_music_synchronization, m_tcpCtr, &tcpController::synchronize_musicFile);
    connect(m_tcpCtr, &tcpController::evoke_homePage_addItem, this, &homePage::deviceList_addDevice);
    connect(m_tcpCtr, &tcpController::evoke_homePage_RemoveItem, this, &homePage::deviceList_removeDevice);
    for (auto box: m_boxList)
    {
        connect(box, &QComboBox::currentIndexChanged, [=](qint64 row) {
            qDebug() << "index changed" << endl;
            QString qstrName = box->objectName();
            QString qstrChannelName = qstrName.section('_', 1, 1);
            if (m_toChannelIndex.find(qstrChannelName) == m_toChannelIndex.end())
            {
                QMessageBox::critical(this, tr("set channel"), tr("invalid channel name"));
                return;
            }
            qint64 nChannelIndex = m_toChannelIndex[qstrChannelName];
            qint64 nLastId = m_lastId.at(nChannelIndex);
            qDebug() << "current Text: " << box->currentText() << endl;
            qDebug() << "last id = " << nLastId << endl;
            if (box->currentText() == "null") // if current select null
            {
                /* revert status of other combobox's corresponding item to selectable */
                for (auto otherBox: m_boxList) // revert to selectable
                {
                    if ((otherBox == box) or (otherBox->findText(QString::number(nLastId)) == -1 )) continue;
                    SetComboBoxItemEnabled(otherBox, otherBox->findText(QString::number(nLastId)), true);
                }
                m_lastId[nChannelIndex] = -1;
            }
            else
            {
                qint64 id = box->currentText().toInt();
                if (nLastId == id) return;
                for (auto otherBox: m_boxList)
                {
                    if (otherBox == box) continue;
                    /* set status of other combobox's item that has the same id to unselectable*/
                    SetComboBoxItemEnabled(otherBox, otherBox->findText(QString::number(id)), false);
                    /* revert status of other combobox's corresponding item to selectable */
                    if ((nLastId != -1) and (otherBox->findText(QString::number(nLastId)) != -1 ))
                    {
                        SetComboBoxItemEnabled(otherBox, otherBox->findText(QString::number(nLastId)), true);
                    }
                }

                m_lastId[nChannelIndex] = id;
                qDebug() << "evok! id = " << id << endl;
                emit evoke_tcpCtr_setChannel(id, nChannelIndex);
            }
        });
    }
    connect(this, &homePage::evoke_tcpCtr_setChannel, m_tcpCtr, &tcpController::setChannel);

    /* remind user */
    QMessageBox::warning(this, tr("注意"), tr("请所有音频文件中不要出现#以及_字符，否则会导致系统异常出现未定义行为，一定要注意，如果音频文件中出现了#字符或_字符，请自行改名或者系统回强制替换未其他字符"));
}

homePage::~homePage()
{
    delete ui;
    if (m_tcpCtr)
    {
        delete m_tcpCtr;
        m_tcpCtr = nullptr;
    }
    if (m_MedaProces)
    {
        delete m_MedaProces;
        m_MedaProces = nullptr;
    }
}

void homePage::on_pushButtonMediaProcess_clicked()
{
    m_MedaProces->show();
}

void homePage::on_pushButtonController_clicked()
{
    m_tcpCtr->show();
}


void homePage::on_pushButtonChangeLayout_clicked()
{
    qint64 nCurIndex = ui->stackedWidget->currentIndex();
    if (nCurIndex == 0)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void homePage::deviceList_addDevice(qint64 id)
{
    if (id < 0)
    {
        QMessageBox::warning(this, tr(__FUNCTION__), tr("invalid id"));
        return;
    }
//    ui->listWidget_devices->addItem(QString::number(id));
    QListWidgetItem *item = new QListWidgetItem(QString::number(id));
    item->setBackground(QBrush(Qt::black));
    ui->listWidget_devices->sortItems();
    QString qstrFamily = "Segoe UI";
    item->setFont(QFont(qstrFamily, 16, 6));
    item->setTextAlignment(Qt::AlignHCenter);
    ui->listWidget_devices->addItem(item);
    addItem2AllChannelComboBox(QString::number(id));
}

void homePage::deviceList_removeDevice(qint64 id)
{
    if (id < 0)
    {
        QMessageBox::warning(this, tr(__FUNCTION__), tr("invalid id"));
        return;
    }
    for (int row = 0 ; row < ui->listWidget_devices->count() ; ++ row)
    {
        QListWidgetItem *item = ui->listWidget_devices->item(row);
        if (item->text().toInt() == id)
        {
            ui->listWidget_devices->takeItem(row);
            break;
        }
    }
    ui->listWidget_devices->sortItems();
    removeItemFromAllChannelComboBox(QString::number(id));
}

bool homePage::removeItemFromAllChannelComboBox(QString text)
{
    qint64 nDeleteRow, nCurRow;
    for (auto box: m_boxList)
    {
        nDeleteRow = box->findText(text);
        nCurRow = box->currentIndex();
        if (nDeleteRow == nCurRow) box->setCurrentIndex(box->findText("null"));
        box->removeItem(nDeleteRow);
    }
    return true;
}

bool homePage::addItem2AllChannelComboBox(QString text)
{
    for (auto box: m_boxList)
    {
        box->addItem(text);
    }
    return true;
}

void homePage::SetComboBoxItemEnabled(QComboBox *comboBox, int index, bool enabled)
{
    auto *model = qobject_cast<QStandardItemModel *>(comboBox->model());
    assert(model);
    if (!model) return;

    auto *item = model->item(index);
    assert(item);
    if (!item) return;
    item->setEnabled(enabled);
}
