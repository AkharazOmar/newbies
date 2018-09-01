#include "http.h"

#include <QFile>
#include <QDataStream>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTextCodec>
#include <QTextDecoder>
#include <QTextDocument>
#include <QDomDocument>


#include <cmath>
#include <iostream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>


Http::Http(QObject * parent):
    QObject (parent)
{
    nam = std::make_unique<QNetworkAccessManager>();
    // Build cookie
    auto cJar = new QNetworkCookieJar(nam.get());
    QList<QNetworkCookie> cookieListNewBies = {
        QNetworkCookie(QByteArray("PHPSESSID"), QByteArray("77fd07838a41ec4a41249de9c3bf2eaa")),
        QNetworkCookie(QByteArray("SMFCookie89"), QByteArray("a%3A4%3A%7Bi%3A0%3Bs%3A5%3A%2256039%22%3Bi%3A1%3Bs%3A40%3A%22c33f9399032bf4b7d3e9201e69923a229cddfa00%22%3Bi%3A2%3Bi%3A1724345896%3Bi%3A3%3Bi%3A0%3B%7D")),
    };
    cJar->setCookiesFromUrl(cookieListNewBies, QUrl("https://www.newbiecontest.org"));
    QList<QNetworkCookie> cookieListRootMe = {
        QNetworkCookie(QByteArray("PHPSESSID"), QByteArray("s8e2dl9822fg1o3r3f9kl8qck2")),
    };
    cJar->setCookiesFromUrl(cookieListNewBies, QUrl("http://challenge01.root-me.org"));
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

void Http::sendGetRequest(QUrl &&request)
{
    auto reply = nam->get(QNetworkRequest(request));
    reply->ignoreSslErrors();
}

void Http::sendPostRequest(QUrl &&request,const QUrlQuery &postData)
{
    QNetworkRequest nRequest(request);
    nRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                       "application/x-www-form-urlencoded");
    auto reply = nam->post(nRequest, postData.toString(QUrl::FullyEncoded).toUtf8());
    reply->ignoreSslErrors();
}

void Http::onFinished(QNetworkReply * reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QTextDocument textDo;
        QDomDocument domDo("root-me");
        QByteArray byteData = reply->readAll();
        textDo.setHtml(byteData.data());
        domDo.setContent(textDo.toHtml());
        qDebug().noquote() << domDo.toString();

        if (reply->url().toString() == "https://www.newbiecontest.org/epreuves/prog/prog1.php") {
            sendGetRequest(QUrl("https://www.newbiecontest.org/epreuves/prog/verifpr1.php?solution=" + textDo.toPlainText().split(" ").last()));
        } else if (reply->url().toString() == "https://www.newbiecontest.org/epreuves/prog/prog3_1.php") {
            a = textDo.toPlainText().split(" ").last().toInt();
            sendGetRequest(QUrl("https://www.newbiecontest.org/epreuves/prog/prog3_2.php"));
        } else if (reply->url().toString() == "https://www.newbiecontest.org/epreuves/prog/prog3_2.php") {
            b = textDo.toPlainText().split(" ").last().toInt();
            int result = sqrt(a)*b;
            qDebug() << "FIXME value computed: " << result;
            sendGetRequest(QUrl("https://www.newbiecontest.org/epreuves/prog/verifpr3.php?solution=" + QString::number(result)));
        } else if (reply->url().toString() == "http://challenge01.root-me.org/programmation/ch8/" && (textDo.toPlainText().contains("Trop tard") || textDo.toPlainText().contains("retente ta chance"))) {
            // FIXME read img from html
            QDomNode &&n = domDo.documentElement().firstChild();
            if (n.isNull()) {
                std::cout << "FIXME domDo don't have children" << std::endl;
                std::cout << "FIXME: " << qPrintable(domDo.toString()) << std::endl;
            }

            while(!n.isNull()) {
                QDomElement &&e = n.toElement();
                if (!e.isNull()) {
                    std::cout << "e.tagName: " << qPrintable(e.tagName()) << std::endl;
                    if (e.tagName() == "body") {
                        n = e.firstChild();
                    } else if (e.tagName() == "p") {
                        n = e.firstChild();
                    } else if (e.tagName() == "img") {
                        QByteArray binImg = QByteArray::fromBase64(e.attribute("src").split(",")[1].toStdString().c_str());
                        tesseract::TessBaseAPI tessa;
                        tessa.Init(nullptr, "cym");

                        QFile captcha("/tmp/captcha.png");
                        if (!captcha.open(QIODevice::WriteOnly)) {
                            return;
                        }

                        captcha.write(binImg);
                        captcha.close();
//                        Pix *image = pixRead("/tmp/captcha.png");

                        Pix * image = pixReadMem(reinterpret_cast<unsigned char *>(binImg.data()), binImg.size());
                        tessa.SetImage(image);
                        tessa.Recognize(nullptr);
                        QString result(tessa.GetUTF8Text());

                        qDebug() << "FIXME: result: " << result;
                        QUrlQuery postData;
                        postData.addQueryItem("cametu", result.remove(result.size() -1, 1));
                        sendPostRequest(QUrl("http://challenge01.root-me.org/programmation/ch8/"), postData);
                        tessa.End();
                        pixDestroy(&image);

                    }
                }
                n = n.nextSibling();
            }
        } else {
            QCoreApplication::quit();
        }
    } else {
        qDebug() << "Error when received the response : " << reply->error();
    }
}
