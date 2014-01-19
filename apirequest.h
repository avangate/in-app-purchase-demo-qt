#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <QObject>
#include <QJsonObject>
#include <QVariant>

class APIRequest : public QObject
{
    Q_OBJECT

    Q_PROPERTY (ushort id READ id WRITE setId)
    Q_PROPERTY (float jsonrpc READ jsonrpc WRITE setJsonRPC)
    Q_PROPERTY (QString method READ method WRITE setMethod)
    Q_PROPERTY (QVariantList params READ params WRITE setParams)

private:
    QString _method;
    QVariantList _params;
    ushort _id;
    float _jsonrpc;

public:
    explicit APIRequest(QObject *parent = 0);
//    explicit APIRequest(const QString& method,  QVariantList& params, const ushort id, QObject *parent = 0);
    QJsonObject toJsonObject();
    static APIRequest* createRequest (const QString& method,  QVariantList &params);
    static APIRequest* createRequest (const char* method, QVariantList &params);

    void setMethod (const QString &method);
    void setParams ( QVariantList &params);
    void setJsonRPC (const float version);
    void setId (const ushort id);

    QString method() const;
    QVariantList params();
    ushort id() const;
    float jsonrpc() const;
signals:

public slots:

};

#endif // APIREQUEST_H
