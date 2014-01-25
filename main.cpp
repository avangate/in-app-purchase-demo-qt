#include <QApplication>
#include <QDebug>

#include "config.h"
#include "order.h"
#include "response.h"
#include "billingdetails.h"
#include "mainwindow.h"

using namespace AvangateAPI;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Order* order = new Order(Config::getUrl());

    MainWindow w;

    QObject::connect(order, &Order::signalError, &w, &MainWindow::slotError);
    QObject::connect(order, &Order::signalSetupFinished, &w, &QMainWindow::show);
    QObject::connect(order, &Order::signalSessionStarted, &w, &MainWindow::slotSetSession);
    QObject::connect(order, &Order::signalSessionStarted, [=](QString _sessionHash) {
        qDebug() << "Session started [" << _sessionHash << "]";

        order->setLanguage("en");
        order->setCurrency("eur");

        BillingDetails* b = new BillingDetails();
        b->setFirstName("Marius");
        b->setLastName("Orcsik");
        b->setEmail("marius.orcsik@avangate.com");
        b->setCity("Bucharest");
        b->setAddress("Some Street, no 666");
        b->setCountry("ro");
        b->setState("Bucharest");
        b->setPostalCode("101222");

        order->setBillingDetails(b);

        QStringList Options;
        Options << "2ys" << "250gbstorage1";
        order->addProduct(4553316, 1, Options);

        emit order->signalSetupFinished(_sessionHash);
    });
    QObject::connect(order, &Order::signalError, [=](Response *response) {
        qDebug() << "Error: id[" << response->id () << "]" << response->error ()->code << response->error ()->message;
    });

    //QObject::connect(&w, &MainWindow::signalError ,order, &Order::slotError, &w, &MainWindow::slotError);

    order->login(Config::getMerchantCode(), Config::getSecretKey());
    return a.exec();
}
