#ifndef CODECODESYS_H
#define CODECODESYS_H

#include "../public/macro.h"
#include <QObject>
#include <QMessageBox>
#include <QDebug>

class codecodeSys : public QObject
{
    Q_OBJECT
public:
    explicit codecodeSys(QObject *parent = nullptr);

public slots:
    static QString code(QString type, QString name_actObj, QString size_actName, QString num_actVal);

static qint64 decode_type(QString const &ins, QString &msg_error); // type  of instruction decode

static qint64 decode_file(QString const &ins, QString &name, qint64 &uSize, qint64 &num, QString &msg_error); // file type ins decode

static qint64 decode_act(QString const &ins, qint64 &uAct_obj, qint64 &uAct_name, qint64 &uAct_val, QString &msg_error);

static QString INS_generator(QString const &qstrIns);

signals:
};

#endif // CODECODESYS_H
