#include <QCoreApplication>

#include "apiorder.h"
#include "billingdetails.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const QString Identifier = "AVANTEST";
    const QString SecretKey = "5&rl6N3%eT1O5(oW9~^I";

    APIOrder* order = new APIOrder(QUrl("https://api.avangate.com/order/2.0/rpc/"),
                                   Identifier, SecretKey);

//    order->setLanguage("en");
//    order->setCurrency("eur");
//    order->setIP();
    //order->setCurrency("eur");

    BillingDetails b;
    b.setFirstName("Marius");
    b.setLastName("Orcsik");
    b.setEmail("marius.orcsik@avangate.com");

    order->setBillingDetails(&b);

    return a.exec();
}
