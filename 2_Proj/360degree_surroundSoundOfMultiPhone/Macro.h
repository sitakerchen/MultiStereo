#ifndef MACRO_H
#define MACRO_H
#include <QObject>

const QString FFMPEG_SPLIT_2 = "ffmpeg -i %1 -filter_complex \"[0:a]channelsplit=channel_layout=stereo[left][right]\" -map \"[left]\" left.wav -map \"[right]\" right.wav";

#endif // MACRO_H
