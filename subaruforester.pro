QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    canbus.cpp \
    dlc.cpp \
    ecm.cpp \
    ecu.cpp \
    elm327.cpp \
    elm327config.cpp \
    obdlink.cpp \
    subaruforester.cpp \
    tcm.cpp \
    main.cpp \
    console.cpp \
    settingsdialog.cpp \
    mainwindow.cpp \

HEADERS += \
    can.h \
    canbus.h \
    dlc.h \
    ecm.h \
    ecu.h \
    elm.h \
    elm327.h \
    elm327config.h \
    obd.h \
    obdlink.h \
    subaru.h \
    subaruforester.h \
    tcm.h \
    console.h \
    settingsdialog.h \
    mainwindow.h

FORMS += \
    settingsdialog.ui \
    mainwindow.ui

RESOURCES += \
    terminal.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
