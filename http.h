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

    void sendGetRequest(QUrl &&request);

    void sendPostRequest(QUrl &&request, const QUrlQuery &postData);

    void onFinished(QNetworkReply *reply);

    void checkEncrypted(QNetworkReply *reply);

private:
    int a, b = 0;
    std::unique_ptr<QNetworkAccessManager> nam;
};

#endif // HTTP_H
