#-------------------------------------------------
#
# Project created by QtCreator 2014-01-18T12:13:01
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = cart-demo-qt
CONFIG   += console crypto
CONFIG   += c++11

CONFIG   -= app_bundle

LIBS += -lqca-qt5
#LIBS += -L/usr/local/lib/ -l/usr/local/lib/qca.so

TEMPLATE = app


SOURCES += main.cpp \
    apiorder.cpp \
    apirequest.cpp \
    apiresponse.cpp \
    billingdetails.cpp \
    order.cpp

HEADERS += \
    apiorder.h \
    apirequest.h \
    apiresponse.h \
    billingdetails.h \
    order.h


#INCLUDEPATH += /usr/local/include/QtCrypto
#DEPENDPATH += /usr/local/include/QtCrypto
