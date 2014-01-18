#ifndef BILLINGDETAILS_H
#define BILLINGDETAILS_H

#include <QObject>

class BillingDetails : public QObject
{
    Q_OBJECT

    Q_PROPERTY (QString FirstName READ FirstName WRITE setFirstName)
    Q_PROPERTY (QString LastName READ LastName WRITE setLastName)
    Q_PROPERTY (QString Email READ Email WRITE setEmail)
    Q_PROPERTY (QString City READ City WRITE setCity)
    Q_PROPERTY (QString State READ State WRITE setState)
    Q_PROPERTY (QString Country READ Country WRITE setCountry)
    Q_PROPERTY (QString PostalCode READ PostalCode WRITE setPostalCode)

private:
    QString _FirstName;
    QString _LastName;
    QString _Email;
    QString _City;
    QString _State;
    QString _Country;
    QString _PostalCode;

public:
    explicit BillingDetails(QObject *parent = 0);

    void setFirstName(QString FirstName);
    void setLastName(QString LastName);
    void setEmail(QString Email);
    void setCity(QString City);
    void setState(QString State);
    void setCountry(QString CountryCode);
    void setPostalCode(QString PostalCode);

    QString FirstName();
    QString LastName();
    QString Email();
    QString City();
    QString State();
    QString Country();
    QString PostalCode();

signals:

public slots:

};

#endif // BILLINGDETAILS_H
