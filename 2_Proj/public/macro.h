#ifndef MACRO_H
#define MACRO_H
#include <QObject>
using qs = QString;
/* ffmpeg */
const QString FFMPEG_SPLIT_2 = "ffmpeg -i %1 -filter_complex \"[0:a]channelsplit=channel_layout=stereo[left][right]\" -map \"[left]\" left.wav -map \"[right]\" right.wav";

/* android */
const QString PATH_ANDROID_ROOT = "/storage/emulated/0/";
const QString PATH_ANDROID_BASE = "/storage/emulated/0/Documents/";
const QString PATH_ANDROID_APP_HOME = "/storage/emulated/0/Documents/Client/";
const QString PATH_ANDROID_APP_MUSIC = "/storage/emulated/0/Documents/Client/Music/";

/* error code */


/* action object */
const qint64 ACT_OBJECT_PLAYER = 0;
const qint64 ACT_OBJECT_HOMEPAGE = 1;

/* action Name */
const qint64 ACT_NAME_PLAYBACK = 0;
const qint64 ACT_NAME_ASSIGN_ID = 1;

/* instructions */

/*
 *     0       1          2                  3                   4            5
 *  (length#)type##name/actionObject##fileSize/actionName##channelNumber/actionValue##dueTime
 */

const QString INS_ = "%1##%2##%3##%4";

// type code
const qint64 TYPE_FILE = 0;
const qint64 TYPE_ACT  = 1;
const qint64 TYPE_MSG  = 2;

// action instructions
//  0         1            2            3
// type##actionObject##actionName##actionValue
const QString INS_PLAY  = qs::number(TYPE_ACT) + "##" + qs::number(ACT_OBJECT_PLAYER) + "##" + qs::number(ACT_NAME_PLAYBACK) + "##" + "1";
const QString INS_PAUSE = qs::number(TYPE_ACT) + "##" + qs::number(ACT_OBJECT_PLAYER) + "##" + qs::number(ACT_NAME_PLAYBACK) + "##" + "0";
const QString INS_ASSIGN_ID = qs::number(TYPE_ACT) + "##" + qs::number(ACT_OBJECT_HOMEPAGE) + "##" + qs::number(ACT_NAME_ASSIGN_ID) + "##" + "%1";

// file instructions
//   0     1       2          3
// type##name##fileSize##channelNumber
const QString INS_FILE_2c = qs::number(TYPE_FILE) + "##" + "%1" + "##" + "%2" + "##" + "2";
const QString INS_FILE_5c = qs::number(TYPE_FILE) + "##" + "%1" + "##" + "%2" + "##" + "5";

// msg instruction
const QString INS_MSG = qs::number(TYPE_MSG) + "##" + "%1##NULL##NULL";

#endif // MACRO_H
