#ifndef ORDER_H
#define ORDER_H

#include <QObject>
#include <QDateTime>
#include <QtNetwork>
#include <QVariantMap>

//#include "client.h"
#include "billingdetails.h"
#include "response.h"
#include "request.h"

namespace AvangateAPI
{


class Order : public QObject
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

//    struct Response {
//        Order::State call;
//        Request* request;
//        Response* response;
//    };

private:

    QString _sessionHash;
    QDateTime _sessionStart;
    ushort _cnt;
    mutable State m_state;
//    QMap<ushort, Response>* m_states;
    QUrl m_url;

    QNetworkAccessManager* networkManager;
    //QNetworkRequest* Request;

    void parseResponse(QJsonDocument jsonDoc);
    void executeRequest (const QString method, QVariantList *params);

public:
    explicit Order(QUrl url, QObject *parent = 0);

    Order::State state();
//    QMap<ushort, Response>* states();

    static QString getCallMethod(Order::State m_state);

    void login(const QString Identifier, const QString SecretKey);
/**/
    void setLanguage (const QString IsoLang);
    void setCountry (const QString IsoCountry);
    void setCurrency (const QString IsoCurrency);
    void setIP (const QString IP);
    void addProduct (const quint32 ProductId, quint8 Quantity, QStringList PriceOptions);
    void setBillingDetails (BillingDetails *Billing);
    void placeOrder ();
/**/
signals:
    void signalSessionStarted(QString sessHash);
    void signalBusy(bool) const;
    void signalSuccess(Response*);
    void signalError(Response*);
    void signalSetupFinished();

public slots:
    void slotError(Response*);
    void slotSuccess(Response*);

private slots:
    void handleNetworkData(QNetworkReply *networkReply);
};
}
#endif // ORDER_H
