#ifndef MACRO_H
#define MACRO_H
#include <QObject>
using qs = QString;
/* file channel index */

// 2 channel
const qint64 FILE_CHANNEL_2_LEFT = 1;
const qint64 FILE_CHANNEL_2_RIGHT = 2;

// 5.1 channel
const qint64 FILE_CHANNEL_5_LEFT_FORE = 3; // left fore 左前声道
const qint64 FILE_CHANNEL_5_LEFT = 4; // 左声道
const qint64 FILE_CHANNEL_5_LEFT_BACK = 5; // 左后声道
const qint64 FILE_CHANNEL_5_RIGHT_FORE = 6;
const qint64 FILE_CHANNEL_5_RIGHT = 7;
const qint64 FILE_CHANNEL_5_RIGHT_BACK = 8;

/* file channel name */

// 2 channel
const QString FILE_CHANNEL_NAME_2_LEFT = "2left";
const QString FILE_CHANNEL_NAME_2_RIGHT = "2right";

// 5.1 channel
const QString FILE_CHANNEL_NAME_5_LEFT_FORE = "5leftFor"; // left fore 左前声道
const QString FILE_CHANNEL_NAME_5_LEFT = "5left";
const QString FILE_CHANNEL_NAME_5_LEFT_BACK = "5leftBac";
const QString FILE_CHANNEL_NAME_5_RIGHT_FORE = "5rightFor";
const QString FILE_CHANNEL_NAME_5_RIGHT = "5right";
const QString FILE_CHANNEL_NAME_5_RIGHT_BACK = "5rightBac";


/* ffmpeg */
const QString FFMPEG_SPLIT_2 = "ffmpeg -i %1 -filter_complex \"[0:a]channelsplit=channel_layout=stereo[left][right]\" -map \"[left]\" %2_" + FILE_CHANNEL_NAME_2_LEFT + ".wav -map \"[right]\" %2_" + FILE_CHANNEL_NAME_2_RIGHT + ".wav";

const QString FFMPEG_SPLIT_5 = "ffmpeg -i %1 \
        -filter_complex \"channelsplit=channel_layout=5.1[FL][FR][FC][LFE][BL][BR]\" \
        -map \"[FL]\" %2_" + FILE_CHANNEL_NAME_5_LEFT_FORE + ".wav \
        -map \"[FR]\" %2_" + FILE_CHANNEL_NAME_5_LEFT + ".wav \
        -map \"[FC]\" %2_" + FILE_CHANNEL_NAME_5_LEFT_BACK + ".wav \
        -map \"[LFE]\" %2_" + FILE_CHANNEL_NAME_5_RIGHT_FORE + ".wav \
        -map \"[BL]\" %2_" + FILE_CHANNEL_NAME_5_RIGHT + ".wav \
        -map \"[BR]\" %2_" + FILE_CHANNEL_NAME_5_RIGHT_BACK + ".wav";

const QString FFMPEG_GET_MEDIA_INFO = "ffprobe -v error -show_entries stream=channels,channel_layout -of default=nw=1 \"%1\" ";

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
const qint64 ACT_NAME_REPLAY = 1;
const qint64 ACT_NAME_SET_VOLUME= 2;
const qint64 ACT_NAME_ASSIGN_ID = 3;

const qint64 ACT_NAME_ASSIGN_CHANNEL = 10;
const qint64 ACT_NAME_SET_SOURCE = 11;
const qint64 ACT_NAME_SET_POS = 12;

/* instructions */

/*
 *            0            1                  2              \
 *  (length#)type##name/actionObject##fileSize/actionName##  \
 *              3                4           5       6
 *  channelNumber/actionValue##baseDelay#sendTime#sendDelay
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
// type##name##fileSize##AmountOfChannels
const QString INS_FILE = qs::number(TYPE_FILE) + "##" + "%1" + "##" + "%2" + "##" + "%3";
const QString INS_FILE_2c = qs::number(TYPE_FILE) + "##" + "%1" + "##" + "%2" + "##" + "2";
const QString INS_FILE_5c = qs::number(TYPE_FILE) + "##" + "%1" + "##" + "%2" + "##" + "5";

// msg instruction
const QString INS_MSG = qs::number(TYPE_MSG) + "##" + "%1##NULL##NULL";

#endif // MACRO_H
