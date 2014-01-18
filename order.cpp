#include "order.h"

Order::Order(QObject *parent) :
    QObject(parent)
{
}

void Order::setRefNo(QString RefNo)
{
    _RefNo = RefNo;
}
void Order::setStatus(QString Status)
{
    _Status = Status;
}
void Order::setApproveStatus(QString ApproveStatus)
{
    _ApproveStatus = ApproveStatus;
}
void Order::setError(QString Error)
{
    _Error = Error;
}
void Order::setRecurringEnabled(bool RecurringEnabled)
{
    _RecurringEnabled = RecurringEnabled;
}

QString Order::RefNo()
{
    return _RefNo;
}
QString Order::Status()
{
    return _Status;
}
QString Order::ApproveStatus()
{
    return _ApproveStatus;
}
QString Order::Error()
{
    return _Error;
}
bool Order::RecurringEnabled()
{
    return _RecurringEnabled;
}

