#ifndef IDENTITYCONTROLLER_H
#define IDENTITYCONTROLLER_H

#include <mutex>
#include <QObject>
#include <QMap>
#include <QMessageBox>
#include <QTcpSocket>
#include <QSet>
#include <QVector>

class identityController : public QObject
{
    Q_OBJECT
public:
    explicit identityController(QObject *parent = nullptr);

    /* identity control */
    static identityController& getInstance(); // return instance of identityController

private:
    static identityController *m_idController;
    QMap<QTcpSocket*, qint64> m_toId; // client to id
    QMap<qint64, QTcpSocket*> m_toSocket; // id to client

    QSet<qint64> idPool;
    qint64 m_MaxSize;
    qint64 getIdFromPool();
    qint64 returnIdToPool(qint64 id);

public slots:
    qint64 id_allocateId(QTcpSocket *client); // allocate a new id when new connexion is established
    bool id_recycleId(QTcpSocket *client); // recycle a id when lost a connexion


private:
    /* release heap(new) storage */
    class Release
    {
    public:
        ~Release()
        {
            if (identityController::m_idController)
            {
                delete identityController::m_idController;
                identityController::m_idController = nullptr;
            }
            qDebug() << "realse finished" << Qt::endl;
        }
    };
    static Release m_MemRelease;
};

#endif // IDENTITYCONTROLLER_H
