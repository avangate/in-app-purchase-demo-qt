#ifndef APICLIENT_H
#define APICLIENT_H

#include "apirequest.h"
#include "apiresponse.h"
#include "billingdetails.h"

#include <QObject>
#include <QUrl>
#include <QDateTime>
#include <QtNetwork>

class APIClient : public QObject
{
    Q_OBJECT

private:
    QString _sessionHash;
    QDateTime _sessionStart;
    ushort _cnt;

    QNetworkAccessManager* networkManager;
    QNetworkRequest* apiRequest;

    const QString _MerchantCode;
    const QString _SecretKey;

    void login(const QString Identifier, const QString SecretKey) const;

public:
    explicit APIClient(const QUrl url, const QString Identifier, const QString SecretKey, QObject *parent = 0);

    bool sessionStarted ();
    bool hasError();
    QString getError();

    void setLanguage (const QString IsoLang);
    void setCountry (const QString IsoCountry);
    void setCurrency (const QString IsoCurrency);
    void setIP (const QString IP);
    void setBillingDetails (const BillingDetails *Billing);
    void placeOrder ();

    void handleError (QString error);
    void handleSuccess (APIResponse *resp);
signals:

private slots:
    void handleNetworkData(QNetworkReply *networkReply);
};

#endif // APICLIENT_H
