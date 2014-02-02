#-------------------------------------------------
#
# Project created by QtCreator 2014-01-18T12:13:01
#
#-------------------------------------------------

TARGET = cart-demo-qt

CONFIG   += qt crypto c++11
CONFIG   -= app_bundle

QT       += core gui network widgets webkitwidgets

LIBS += -lqca-qt5

TEMPLATE = app


SOURCES += main.cpp \
    order.cpp \
    request.cpp \
    response.cpp \
    billingdetails.cpp \
    paymentwindow.cpp \
    mainwindow.cpp \
    paymentdetails.cpp

HEADERS += config.h \
    order.h \
    request.h \
    response.h \
    billingdetails.h \
    paymentwindow.h \
    config.h \
    mainwindow.h \
    paymentdetails.h

FORMS    += mainwindow.ui paymentwindow.ui
    

RESOURCES += \
    resources.qrc
