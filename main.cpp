#include <QApplication>
#include <QMapIterator>
#include <QDebug>

#include "order.h"
#include "response.h"
#include "billingdetails.h"
#include "mainwindow.h"

using namespace AvangateAPI;

int main(int argc, char *argv[])
{

//    const QString Identifier = "AVANTEST";
//    const QString SecretKey = "5&rl6N3%eT1O5(oW9~^I";
//    const QUrl APIUrl("https://api.avangate.com/order/2.0/rpc/");
    const QString Identifier = "CC1234";
    const QString SecretKey = "SECRET_KEY";
    const QUrl APIUrl("http://sandbox101.avangate.local/api/order/2.0/rpc/");

    Order* order = new Order(APIUrl);
    QApplication a(argc, argv);
    MainWindow w;

    QObject::connect(order, &Order::signalError, [=](Response *response) {
        qDebug() << "Error:" << response->id () << response->error ()->code << response->error ()->message;
        //Order::Response* response = order->states()->find(id).value<Order::Response*>();
        //qWarning() << "Error when calling" << Order::getCallMethod(r->call) << "[" << r["error"]["code"] << "]" << r["error"]["message"];
        QApplication::quit();
    });

    QObject::connect(order, &Order::signalSessionStarted, [=](QString _sessionHash) {
        qDebug() << "Session started [" << _sessionHash << "]";

        order->setLanguage("en");
        order->setCurrency("eur");

        BillingDetails* b = new BillingDetails();
        b->setFirstName("Marius");
        b->setLastName("Orcsik");
        b->setEmail("marius.orcsik@avangate.com");

        order->setBillingDetails(b);

        QStringList Options;
        Options << "2ys" << "250gbstorage1";
        order->addProduct(4553316, 1, Options);
    });

    QObject::connect(order, &Order::signalSetupFinished,
                     &w, &QMainWindow::show);

    order->login(Identifier, SecretKey);
    return a.exec();
}
