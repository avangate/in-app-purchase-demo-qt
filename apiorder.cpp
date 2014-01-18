#include "apiorder.h"

#include <QDebug>

APIOrder::APIOrder(QUrl url, const QString Merch, const QString Sk, QObject *parent) :

    QObject(parent)
{
   _client = new APIClient(url, Merch, Sk, this);
}

void APIOrder::setLanguage (const QString IsoLang)
{
    _client->setLanguage(IsoLang);
}

void APIOrder::setCountry (const QString IsoCountry)
{
    _client->setCountry(IsoCountry);
}

void APIOrder::setCurrency (const QString IsoCurrency)
{
    _client->setCurrency(IsoCurrency);
}

void APIOrder::setIP(const QString IP)
{
    _client->setIP (IP);
}

void APIOrder::setBillingDetails(const BillingDetails* Billing)
{
    if (_client->sessionStarted ()) {
        _client->setBillingDetails (Billing);
    }
}

void APIOrder::placeOrder()
{
    if (_client->sessionStarted ()) {
        _client->placeOrder();
    }
}
