#ifndef PAYMENTWINDOW_H
#define PAYMENTWINDOW_H

#include <QDialog>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>

#include "response.h"
#include "order.h"

namespace Ui {
class PaymentWindow;
}


namespace AvangateAPI
{

class PaymentWindow : public QDialog
{
    Q_OBJECT

private:
    ushort m_id;
    QString _session;

public:
    explicit PaymentWindow(ushort callId, QWidget *parent = 0);
    ~PaymentWindow();

    void slotUrlChanged (const QUrl &url);

private:
    Ui::PaymentWindow *ui;

signals:
    void signalError(Response*, Order::State);
    void signalSuccess(Response*, Order::State);
    void signalOrderPlaced(QString, QString);
    void signalPaymentDetailsAdded();

public slots:
    void slotUnsupportedContent (QNetworkReply * reply);
    void slotHandleReply (QNetworkReply * reply);
    void slotSetSession (QString);
    void slotOrderPlaced (QString, QString);
    void slotPaymentDetailsAdded ();
    void slotSuccess ();
    void slotError (Response* err);

};

}
#endif // PAYMENTWINDOW_H
