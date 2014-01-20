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
        PLACEORDER,
        GETORDER
    };

    struct Response {
        bool status;
        APIOrder::State call;
        APIResponse::Error* error;
    };

private:

    QString _sessionHash;
    QDateTime _sessionStart;
    ushort _cnt;
    mutable State m_state;
    QMap<ushort, APIOrder::Response>* m_states;

    QNetworkAccessManager* networkManager;
    QNetworkRequest* apiRequest;

    void parseResponse(QJsonDocument* r);
    void executeRequest (const QString method, QVariantList *params);

public:
    explicit APIOrder(QUrl url, QObject *parent = 0);

    APIOrder::State state();
    QMap<ushort, APIOrder::Response>* states();

    static QString getCallMethod(APIOrder::State m_state);

    void login(const QString Identifier, const QString SecretKey);

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
    void signalSuccess(ushort, APIOrder::Response*);
    void signalError(ushort, APIOrder::Response*);

public slots:
    void slotError(const ushort, Response *);
    void slotSuccess(const ushort, Response *);

private slots:
    void handleNetworkData(QNetworkReply *networkReply);
};

#endif // APIORDER_H
