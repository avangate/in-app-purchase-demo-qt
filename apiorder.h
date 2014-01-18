#ifndef APIORDER_H
#define APIORDER_H

#include <QObject>
#include <QDateTime>

#include "apiclient.h"

class APIOrder : public QObject
{
    Q_OBJECT

private:
    APIClient* _client;
    QString _sessionHash;

public:
    explicit APIOrder(QUrl url, const QString Merch, const QString Sk, QObject *parent = 0);

//    QString login (const QString Identifier, const QString SecretKey);

    void setLanguage (const QString IsoLang);
    void setCountry (const QString IsoCountry);
    void setCurrency (const QString IsoCurrency);
    void setIP (const QString IP);
    void setBillingDetails (const BillingDetails *Billing);
    void placeOrder ();

signals:

public slots:

};

#endif // APIORDER_H
