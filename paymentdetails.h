#ifndef PAYMENTDETAILS_H
#define PAYMENTDETAILS_H

#include <QObject>

namespace AvangateAPI
{

class PaymentDetails : public QObject
{
    Q_OBJECT
private:
    QString _Type;
    QString _Currency;
    QString _CustomerIP;
//    PaymentMethod _PaymentMethod; // @TODO

public:
    explicit PaymentDetails(QObject *parent = 0);

    QString type();
    void setType(QString type);

    QString currency();
    void setCurrency (QString currency);

    QString customerIP();
    void setCustomerIP (QString ip);
signals:

public slots:

};

}

#endif // PAYMENTDETAILS_H
