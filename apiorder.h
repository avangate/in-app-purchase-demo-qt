#ifndef APIORDER_H
#define APIORDER_H

#include <QObject>
#include <QDateTime>
#include <QtNetwork>
#include <QVariantMap>

//#include "apiclient.h"
#include "billingdetails.h"
#include "apiresponse.h"
#include "apirequest.h"

class APIOrder : public QObject
{
    Q_OBJECT

public:
    enum State {
        IDLE,
        LOGIN = 1,
        SETLANGUAGE,
        SETCOUNTRY,
        SETCURRENCY,
        SETIP,
        SETBILLINGDETAILS,
        SETPAYMENTDETAILS,
        ADDPRODUCT,
        PLACEORDER
    };

private:

    QString _sessionHash;
    QDateTime _sessionStart;
    ushort _cnt;
    mutable State m_state;

    QNetworkAccessManager* networkManager;
    QNetworkRequest* apiRequest;

    static QString getCallMethod(APIOrder::State m_state);
    static APIResponse* fromJsonDocument(QJsonDocument* r, APIOrder::State m_state);
    void executeRequest (const QString method, QVariantList *params) const;

public:
    explicit APIOrder(QUrl url, QObject *parent = 0);

    APIOrder::State state();
    void login(const QString Identifier, const QString SecretKey) const;

    void setLanguage (const QString IsoLang);
    void setCountry (const QString IsoCountry);
    void setCurrency (const QString IsoCurrency);
    void setIP (const QString IP);
    void addProduct (const quint32 ProductId, quint8 Quantity, QStringList PriceOptions);
    void setBillingDetails (const BillingDetails *Billing);
    void placeOrder ();

signals:
    void signalSessionStarted(QString sessHash);
    void signalBusy(bool) const;
    void signalError(APIResponse::Error *error);
    void signalSuccess (APIResponse *resp);

public slots:
    void slotError(APIResponse::Error *error);
    void slotSuccess (APIResponse *_response);

private slots:
    void handleNetworkData(QNetworkReply *networkReply);
};

#endif // APIORDER_H
