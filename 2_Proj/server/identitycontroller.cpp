#include "identitycontroller.h"
using Qt::endl;
identityController *identityController::m_idController;
identityController::Release identityController::m_MemRelease;

identityController::identityController(QObject *parent)
    : QObject{parent}
{
    m_MaxSize = 6;
    for (int i = 0 ; i < 6 ; ++ i)
    {
        idPool.insert(i);
    }
    qDebug() << "idPool" << idPool << endl;
}

identityController &identityController::getInstance()
{
    if (m_idController)
    {
        return *m_idController;
    }
    std::once_flag oc;
    std::call_once(oc, [&]{m_idController = new identityController();});

    return *m_idController;
}

qint64 identityController::getIdFromPool()
{
    if (idPool.size() == 0)
    {
        QMessageBox::warning(nullptr, tr(__FUNCTION__), tr("id exhausted"));
    }
    qint64 nMin = m_MaxSize;
    for (auto id: idPool) nMin = qMin(nMin, id);
    bool ok = idPool.remove(nMin);
    if (!ok)
    {
        QMessageBox::critical(nullptr, tr(__FUNCTION__), tr("get id fail!"));
        return -1;
    }
    return nMin;
}

qint64 identityController::returnIdToPool(qint64 id)
{
    if (idPool.size() >= m_MaxSize )
    {
        QMessageBox::critical(nullptr, tr(__FUNCTION__), tr("too much, return id fail!"));
        return -1;
    }
    if (idPool.find(id) != idPool.constEnd()) // if find item id
    {
        QMessageBox::critical(nullptr, tr(__FUNCTION__), tr("id already existed, return id fail!"));
        return -2;
    }
    idPool.insert(id);
    return 0;
}

qint64 identityController::id_allocateId(QTcpSocket *client)
{
    qint64 id = getIdFromPool();
    m_toId[client] = id;
    m_toSocket[id] = client;
    return id;
}

bool identityController::id_recycleId(QTcpSocket *client)
{
    returnIdToPool(m_toId[client]);
    bool ok1 = m_toSocket.remove(m_toId[client]);
    bool ok2 = m_toId.remove(client);
    if (!ok1 or !ok2)
    {
        QMessageBox::critical(nullptr, tr(__FUNCTION__), tr("remove id fail!"));
        return false;
    }
    return true;
}

