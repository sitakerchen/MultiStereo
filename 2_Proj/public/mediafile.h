#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <QWidget>
#include <QFile>
#include <QTimer>

class mediaFile : public QWidget
{
    Q_OBJECT
public:
    explicit mediaFile(QWidget *parent = nullptr);

public:
QFile file; // file object
QString fileName; // file name
qint64 fileSize; // file size
qint64 recvSize; // received file size
qint64 sendSize; // send file size

bool isStart; // receive head file flag

protected:
qint8  pad1, pad2, pad3, pad4, pad5, pad6, pad7; // padding QTimer  class

public:
QTimer timer; // timer
};

#endif // MEDIAFILE_H
