QT       += core gui network concurrent
QT  += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../public/codecodesys.cpp \
    ../public/mediafile.cpp \
    homePage.cpp \
    main.cpp \
    musicplayer.cpp \
    tcpclient.cpp

HEADERS += \
    ../public/codecodesys.h \
    ../public/macro.h \
    ../public/mediafile.h \
    homePage.h \
    musicplayer.h \
    tcpclient.h

FORMS += \
    homePage.ui \
    musicplayer.ui \
    tcpclient.ui

TARGET = client

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    imagefile.qrc
