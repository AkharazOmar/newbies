#include "http.h"
#include <cmath>
#include <iostream>

#include <QTextDocument>
#include <QTextCodec>
#include <QTextDecoder>
#include <QCoreApplication>
#include <QNetworkCookie>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>



#include <QNetworkProxy>


Http::Http(QObject * parent):
    QObject (parent)
{
    nam = std::make_unique<QNetworkAccessManager>();
    // Build cookie
    auto cJar = new QNetworkCookieJar(nam.get());
    QList<QNetworkCookie> cookieList = {
        QNetworkCookie(QByteArray("PHPSESSID"), QByteArray("77fd07838a41ec4a41249de9c3bf2eaa")),
        QNetworkCookie(QByteArray("SMFCookie89"), QByteArray("a%3A4%3A%7Bi%3A0%3Bs%3A5%3A%2256039%22%3Bi%3A1%3Bs%3A40%3A%22c33f9399032bf4b7d3e9201e69923a229cddfa00%22%3Bi%3A2%3Bi%3A1724345896%3Bi%3A3%3Bi%3A0%3B%7D")),
    };
    cJar->setCookiesFromUrl(cookieList, QUrl("https://www.newbiecontest.org"));
    nam->setCookieJar(cJar);

//    Set proxy. Used with burp to anylyse the trame http.
//    QNetworkProxy proxy(
//                QNetworkProxy::HttpProxy,
//                "localhost",
//                8080);
//    nam->setProxy(proxy);

    connect(nam.get(), &QNetworkAccessManager::finished,
            this, &Http::onFinished);
    connect(nam.get(), &QNetworkAccessManager::encrypted,
            this, &Http::checkEncrypted);
}

void Http::checkEncrypted(QNetworkReply */*reply*/)
{
    // qDebug() << "FIXME: encrypted received";
}

void Http::sendRequest(QUrl &&request)
{
    auto reply = nam->get(QNetworkRequest(request));
    reply->ignoreSslErrors();
}

void Http::onFinished(QNetworkReply * reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QTextDocument textDo;
        textDo.setHtml(reply->readAll().data());
        qDebug() << textDo.toPlainText();
        if (reply->url().toString() == "https://www.newbiecontest.org/epreuves/prog/prog1.php") {
            sendRequest(QUrl("https://www.newbiecontest.org/epreuves/prog/verifpr1.php?solution=" + textDo.toPlainText().split(" ").last()));
        } else if (reply->url().toString() == "https://www.newbiecontest.org/epreuves/prog/prog3_1.php") {
            a = textDo.toPlainText().split(" ").last().toInt();
            sendRequest(QUrl("https://www.newbiecontest.org/epreuves/prog/prog3_2.php"));
        } else if (reply->url().toString() == "https://www.newbiecontest.org/epreuves/prog/prog3_2.php") {
            b = textDo.toPlainText().split(" ").last().toInt();
            int result = sqrt(a)*b;
            qDebug() << "FIXME value computed: " << result;
            sendRequest(QUrl("https://www.newbiecontest.org/epreuves/prog/verifpr3.php?solution=" + QString::number(result)));
        }else {
            QCoreApplication::quit();
        }
    } else {
        qDebug() << "Error when received the response : " << reply->error();
    }
}
