#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <QWidget>
#include <QFile>
#include <QTimer>

class mediaFile : public QWidget
{
    Q_OBJECT
public:
    explicit mediaFile();
    void reset_status();
public:
QFile file; // file object
QString fileName; // file name
QString filePath; // local path of file
qint64 fileSize; // file size
qint64 recvSize; // received file size
qint64 sendSize; // send file size
qint64 channelNumber; // number of media file


protected:
qint8  pad[7] = {0}; // padding QTimer  class

public:
QTimer timer; // timer
};

#endif // MEDIAFILE_H
