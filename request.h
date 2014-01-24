#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QJsonObject>
#include <QVariant>

namespace AvangateAPI
{

class Request
{
private:
    ushort m_id;
    float m_jsonrpc;
    QString m_method;
    QVariantList m_params;

public:
    explicit Request();
//    explicit Request(const QString& method,  QVariantList& params, const ushort id, QObject *parent = 0);
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
//    static Request* createRequest (const QString& method,  QVariantList &params);
//    static Request* createRequest (const char* method, QVariantList &params);
    QJsonObject jsonObject ();

    void setMethod (const QString &method);
    void setParams ( QVariantList &params);
    void setJsonRPC (const float version);
    void setId (const ushort id);

    QString method() const;
    QVariantList params();
    ushort id() const;
    float jsonrpc() const;
signals:


};
}
#endif // REQUEST_H
