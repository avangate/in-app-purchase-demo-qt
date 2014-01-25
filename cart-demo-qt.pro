#-------------------------------------------------
#
# Project created by QtCreator 2014-01-18T12:13:01
#
#-------------------------------------------------

QT       += core network gui widgets webkitwidgets
QT       -= gui

TARGET = cart-demo-qt
CONFIG   += console crypto
CONFIG   += c++11

CONFIG   -= app_bundle

LIBS += -lqca-qt5
#LIBS += -L/usr/local/lib/ -l/usr/local/lib/qca.so

TEMPLATE = app


SOURCES += main.cpp \
    order.cpp \
    request.cpp \
    response.cpp \
    billingdetails.cpp \
    mainwindow.cpp \
    urnnetworkaccessmanager.cpp

HEADERS += \
    order.h \
    request.h \
    response.h \
    billingdetails.h \
    mainwindow.h \
    urnnetworkaccessmanager.h \
    config.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

#INCLUDEPATH += /usr/local/include/QtCrypto
#DEPENDPATH += /usr/local/include/QtCrypto
