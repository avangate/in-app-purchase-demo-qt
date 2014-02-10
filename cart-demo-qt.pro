#-------------------------------------------------
#
# Project created by QtCreator 2014-01-18T12:13:01
#
#-------------------------------------------------

TARGET = cart-demo-qt

CONFIG   += c++11 qwt
CONFIG   -= app_bundle

QT       -= opengl \
            qml \
            quick \
            multimedia \
            multimediawidgets \
            sensors \
            printsupport \
            sql  
QT       += network widgets webkitwidgets

TEMPLATE = app

SOURCES += main.cpp \
    order.cpp \
    request.cpp \
    response.cpp \
    billingdetails.cpp \
    paymentwindow.cpp \
    mainwindow.cpp \
    paymentdetails.cpp \
    tvplot.cpp


HEADERS += config.h \
    order.h \
    request.h \
    response.h \
    billingdetails.h \
    paymentwindow.h \
    config.h \
    mainwindow.h \
    paymentdetails.h \
    tvplot.h

FORMS    += paymentwindow.ui mainwindow.ui
    
RESOURCES += \
    resources.qrc
