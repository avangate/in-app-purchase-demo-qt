#ifndef CONFIG_H
#define CONFIG_H

#include <QUrl>

class Config
{
public:
    static bool devel;

    static inline const QString getMerchantCode () {
        if (devel) {
            return "MERCHANT_CODE";
        } else {
            return "";
        }
    }
    static inline const QString getSecretKey () {
        if (devel) {
            return "SECRET";
        } else {
            return "";
        }
    }
    static inline const QUrl getUrl () {
        if (devel) {
            return QUrl("https://api.avangate.com/order/2.0/rpc/");
        } else {
            return QUrl("https://api.avangate.com/order/2.0/rpc/");
        }
    }
    static inline const QStringList getPriceOptions() {
            QStringList l;
        if (devel) {
            // add pricing option codes: l << "red";
        } else {
        }
        return l;
    }
    static inline const int getProductId () {
        if (devel) {
            return 1;
        } else {
            return 0;
        }
    }
};

#endif // CONFIG_H
