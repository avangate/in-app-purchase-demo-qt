#ifndef BILLINGDETAILS_H
#define BILLINGDETAILS_H

#include <QObject>

namespace AvangateAPI
{
class BillingDetails : public QObject
{
    Q_OBJECT

    Q_PROPERTY (QString FirstName READ FirstName WRITE setFirstName)
    Q_PROPERTY (QString LastName READ LastName WRITE setLastName)
    Q_PROPERTY (QString Email READ Email WRITE setEmail)
    Q_PROPERTY (QString City READ City WRITE setCity)
    Q_PROPERTY (QString State READ State WRITE setState)
    Q_PROPERTY (QString Country READ Country WRITE setCountry)
    Q_PROPERTY (QString Address READ Address WRITE setAddress)
    Q_PROPERTY (QString PostalCode READ PostalCode WRITE setPostalCode)

private:
    QString _FirstName;
    QString _LastName;
    QString _Email;
    QString _City;
    QString _State;
    QString _Country;
    QString _PostalCode;
    QString _Address;

public:
    BillingDetails(QObject *parent = 0);
    BillingDetails(const BillingDetails &toCopy);
    ~BillingDetails();

    void setFirstName(QString FirstName);
    void setLastName(QString LastName);
    void setEmail(QString Email);
    void setCity(QString City);
    void setState(QString State);
    void setCountry(QString CountryCode);
    void setAddress(QString Address);
    void setPostalCode(QString PostalCode);

    QString FirstName() const;
    QString LastName() const;
    QString Email() const;
    QString City() const;
    QString State() const;
    QString Country() const;
    QString PostalCode() const;
    QString Address() const;

signals:

public slots:

};
}

Q_DECLARE_METATYPE(AvangateAPI::BillingDetails)

#endif // BILLINGDETAILS_H
