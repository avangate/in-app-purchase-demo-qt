#include "billingdetails.h"

using namespace AvangateAPI;

BillingDetails::BillingDetails(QObject *parent) :
    QObject(parent)
{
}

void BillingDetails::setFirstName(QString FirstName)
{
    _FirstName = FirstName;
}
void BillingDetails::setLastName(QString LastName)
{
    _LastName = LastName;
}
void BillingDetails::setEmail(QString Email)
{
    _Email = Email;
}
void BillingDetails::setCity(QString City)
{
    _City = City;
}
void BillingDetails::setState(QString State)
{
    _State = State;
}
void BillingDetails::setCountry(QString CountryCode)
{
    _Country = CountryCode;
}
void BillingDetails::setPostalCode(QString PostalCode)
{
    _PostalCode = PostalCode;
}

QString BillingDetails::FirstName()
{
    return _FirstName;
}
QString BillingDetails::LastName()
{
    return _LastName;
}
QString BillingDetails::Email()
{
    return _Email;
}
QString BillingDetails::City()
{
    return _City;
}
QString BillingDetails::State()
{
    return _State;
}
QString BillingDetails::Country()
{
    return _Country;
}
QString BillingDetails::PostalCode()
{
    return _PostalCode;
}
