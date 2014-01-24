#ifndef URNNETWORKACCESSMANAGER_H
#define URNNETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>


namespace AvangateAPI {

class urnNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    explicit urnNetworkAccessManager(QObject *parent = 0);
};

}
#endif // URNNETWORKACCESSMANAGER_H
