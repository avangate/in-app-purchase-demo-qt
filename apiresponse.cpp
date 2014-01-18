#include "apiresponse.h"
#include <QDebug>

APIResponse::APIResponse(const ushort id, QObject *parent) :
    QObject(parent),
    _id(id)
{
}

ushort APIResponse::id() const
{
    return _id;
}

bool APIResponse::isError ()
{
    return false;
}

QString APIResponse::getError ()
{
    return "";
}

QString APIResponse::getResponse ()
{
    return "";
}

void APIResponse::fromJsonObject(QJsonObject r)
{
    qDebug() << r;
}

QJsonObject APIResponse::toJsonObject()
{

    QVariantMap a;
    a.insert("id", id());

    if (isError()) {
        a.insert ("error", getError());
    } else {
        a.insert ("response", getResponse());
    }

    return QJsonObject::fromVariantMap(a);
}
