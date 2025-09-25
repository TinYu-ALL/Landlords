QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    alarmclock.cpp \
    animationwindow.cpp \
    buttongroup.cpp \
    card.cpp \
    cardpannel.cpp \
    cards.cpp \
    endingpanel.cpp \
    gamecontrol.cpp \
    gamepannel.cpp \
    loading.cpp \
    main.cpp \
    musiccontrol.cpp \
    mybutton.cpp \
    player.cpp \
    playhand.cpp \
    robot.cpp \
    robotgraplord.cpp \
    robotplayhand.cpp \
    scorepannel.cpp \
    strategy.cpp \
    userplayer.cpp

HEADERS += \
    alarmclock.h \
    animationwindow.h \
    buttongroup.h \
    card.h \
    cardpannel.h \
    cards.h \
    endingpanel.h \
    gamecontrol.h \
    gamepannel.h \
    loading.h \
    musiccontrol.h \
    mybutton.h \
    player.h \
    playhand.h \
    robot.h \
    robotgraplord.h \
    robotplayhand.h \
    scorepannel.h \
    strategy.h \
    userplayer.h

FORMS += \
    buttongroup.ui \
    gamepannel.ui \
    scorepannel.ui

# # Default rules for deployment.
# qnx: target.path = /tmp/$${TARGET}/bin
# else: unix:!android: target.path = /opt/$${TARGET}/bin
# !isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc


RC_ICONS = images/logo.ico
