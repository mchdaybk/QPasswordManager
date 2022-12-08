QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    createuser.cpp \
    main.cpp \
    login.cpp \
    manager.cpp \
    passwdsource.cpp

HEADERS += \
    ../../../../OpenSSL-Win64/include/openssl/conf.h \
    ../../../../OpenSSL-Win64/include/openssl/evp.h \
    createuser.h \
    login.h \
    manager.h \
    passwdsource.h

FORMS += \
    createuser.ui \
    login.ui \
    manager.ui \
    passwdsource.ui

###############  openssl icin eklendi ###############

LIBS += -L../../../../OpenSSL-Win64 -llibcrypto-3-x64
LIBS += -L../../../../OpenSSL-Win64 -llibssl-3-x64

INCLUDEPATH += C:/OpenSSL-Win64/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
