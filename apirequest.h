#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <QObject>
#include <QJsonObject>
#include <QVariant>

class APIRequest : public QObject
{
    Q_OBJECT

private:
    QString _method;
    QVariantList _params;
    const ushort _id;
    const float _jsonrpc;

public:
    explicit APIRequest(const QString& method, QVariantList& params, const ushort id, QObject *parent = 0);
    QJsonObject toJsonObject();

    void setMethod (const QString &method);
    void setParams (QVariantList &params);
    void setJsonRPC (const float version);

    QString method() const;
    QVariantList params();
    ushort id() const;
    float jsonrpc() const;
signals:

public slots:

};

#endif // APIREQUEST_H
