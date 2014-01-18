#ifndef ORDER_H
#define ORDER_H

#include <QObject>

class Order : public QObject
{
    Q_OBJECT
    Q_PROPERTY (QString RefNo READ RefNo WRITE setRefNo)
    Q_PROPERTY (QString Status READ Status WRITE setStatus)
    Q_PROPERTY (QString ApproveStatus READ ApproveStatus WRITE setApproveStatus)
    Q_PROPERTY (QString Error READ Error WRITE setError)
    Q_PROPERTY (bool RecurringEnabled READ RecurringEnabled WRITE setRecurringEnabled)

private:
    QString _RefNo;
    QString _Status;
    QString _ApproveStatus;
    QString _Error;
    bool _RecurringEnabled;

public:
    explicit Order(QObject *parent = 0);


    void setRefNo(QString RefNo);
    void setStatus(QString Status);
    void setApproveStatus(QString ApproveStatus);
    void setError(QString Error);
    void setRecurringEnabled(bool RecurringEnabled);

    QString RefNo();
    QString Status();
    QString ApproveStatus();
    QString Error();
    bool RecurringEnabled();

signals:

public slots:

};

#endif // ORDER_H
