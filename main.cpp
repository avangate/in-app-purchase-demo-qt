#include <QCoreApplication>
#include <QDebug>

#include "apiorder.h"
#include "apiresponse.h"
#include "billingdetails.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const QString Identifier = "AVANTEST";
    const QString SecretKey = "5&rl6N3%eT1O5(oW9~^I";
    const QUrl APIUrl("https://api.avangate.com/order/2.0/rpc/");

    APIOrder* order = new APIOrder(APIUrl);

    QObject::connect(order, &APIOrder::signalError, [=](APIResponse::Error *err) {
        qWarning() << "Error [" << err->code << "]" << err->message;
        QCoreApplication::quit();
    });

    order->login(Identifier, SecretKey);

    QObject::connect(order, &APIOrder::signalSessionStarted, [=](QString _sessionHash) {
        qDebug() << "Session started [" << _sessionHash << "]";

        order->setLanguage("en");
        order->setCurrency("eur");

        BillingDetails b;
        b.setFirstName("Marius");
        b.setLastName("Orcsik");
        b.setEmail("marius.orcsik@avangate.com");

        order->setBillingDetails(&b);

        QStringList Options;
        Options << "2ys" << "250gbstorage1";
        order->addProduct(4553316, 1, Options);
    });

    return a.exec();
}
