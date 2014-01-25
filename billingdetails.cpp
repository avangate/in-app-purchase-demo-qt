#include "billingdetails.h"

using namespace AvangateAPI;

BillingDetails::BillingDetails(QObject *parent) :
    QObject(parent)
{
}

BillingDetails::BillingDetails(const BillingDetails& toCopy)
{
    setFirstName(toCopy.FirstName());
    setLastName(toCopy.LastName());
    setEmail(toCopy.Email());
    setCity(toCopy.City());
    setCountry(toCopy.Country());
    setPostalCode(toCopy.PostalCode());
    setAddress(toCopy.Address());
    setState(toCopy.State());
}

BillingDetails::~BillingDetails () {

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
void BillingDetails::setAddress(QString Address)
{
    _Address = Address;
}

QString BillingDetails::FirstName() const { return _FirstName; }
QString BillingDetails::LastName() const { return _LastName; }
QString BillingDetails::Email() const { return _Email; }
QString BillingDetails::City() const { return _City;}
QString BillingDetails::State() const { return _State; }
QString BillingDetails::Country() const { return _Country; }
QString BillingDetails::Address() const { return _Address; }
QString BillingDetails::PostalCode() const { return _PostalCode; }
