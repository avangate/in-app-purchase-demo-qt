#include "paymentdetails.h"

using namespace AvangateAPI;

PaymentDetails::PaymentDetails(QObject *parent) :
    QObject(parent)
{
}


QString PaymentDetails::type()
{
    return _Type;
}

void PaymentDetails::setType(QString type)
{
    _Type = type;
}

QString PaymentDetails::currency()
{
    return _Currency;
}

void PaymentDetails::setCurrency (QString currency)
{
    _Currency = currency;
}

QString PaymentDetails::customerIP()
{
    return _CustomerIP;
}

void PaymentDetails::setCustomerIP (QString ip)
{
    _CustomerIP = ip;
}
