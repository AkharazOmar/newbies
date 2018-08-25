#ifndef HTTP_H
#define HTTP_H

#include <memory>
#include <QObject>
#include <QUrlQuery>
#include <QNetworkCookieJar>
#include <QNetworkAccessManager>

class Http final: public QObject
{
    Q_OBJECT
public:
    explicit Http(QObject *parent=nullptr);

    void sendRequest(QUrl &&request);

    void onFinished(QNetworkReply *reply);

    void checkEncrypted(QNetworkReply *reply);

private:
    std::unique_ptr<QNetworkAccessManager> nam;
};

#endif // HTTP_H
