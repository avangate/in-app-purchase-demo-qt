#ifndef ORDER_H
#define ORDER_H

#include <QObject>
#include <QDateTime>
#include <QtNetwork>
#include <QVariantMap>

#include "paymentwindow.h"
#include "billingdetails.h"
#include "paymentdetails.h"
#include "response.h"
#include "request.h"
#include "config.h"

namespace AvangateAPI
{

class Order : public QObject
{
    Q_OBJECT

public:

    struct CallStatus {
        State call;
        bool status;
    };

private:

    PaymentWindow* w;
    int m_currentState;
    QString _sessionHash;
    QDateTime _sessionStart;
    int _cnt;
    mutable State m_state;
    QMap<int, CallStatus> m_states;
    QUrl m_url;

    QNetworkAccessManager* networkManager;

    void parseResponse(QJsonDocument jsonDoc);
    void executeRequest (const QString method, QVariantList *params);

public:
    explicit Order(QUrl url, QWidget *parent = 0);

    State state();
    //QMap<int, State *> *states();

    static QString getCallMethod(State m_state);
    static QString getStateName(State state);
    static QStringList getActiveStatesLabels(int state);

    void login(const QString Identifier, const QString SecretKey);
/**/
    void setLanguage (const QString IsoLang);
    void setCountry (const QString IsoCountry);
    void setCurrency (const QString IsoCurrency);
    void setIP (const QString IP);
    void addProduct (const quint32 ProductId, quint8 Quantity, QStringList PriceOptions);
    void setBillingDetails (BillingDetails *Billing);
    void setPaymentDetails (PaymentDetails *Payment);
    void setPaymentDetails ();
    void placeOrder ();
/**/
signals:
    void signalSessionStarted(QString sessHash);
    void signalBusy(bool) const;
    void signalSuccess(Response*, State);
    void signalError(Response*, State);
    void signalSetupFinished();

    void signalProductAdded();
    void signalBillingDetailsAdded();
    void signalPaymentDetailsAdded();
    void signalShowPaymentWindow();
    void signalOrderPlaced();

public slots:
    void slotError(Response*, State);
    void slotSuccess(Response*, State);

    void slotProductAdded();
    void slotBillingDetailsAdded();
    void slotPaymentDetailsAdded();
    void slotShowPaymentWindow();
    void slotOrderPlaced();

private slots:
    void handleNetworkData(QNetworkReply *networkReply);
};
}
#endif // ORDER_H
