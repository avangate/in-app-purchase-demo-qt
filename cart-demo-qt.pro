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

LIBS += -lqca

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
