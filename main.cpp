#include <QCoreApplication>

#include "http.h"

#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Http ht(nullptr);

    // Prog1
//    ht.sendGetRequest(QUrl("https://www.newbiecontest.org/epreuves/prog/prog1.php"));

    // Prog3
    // ht.sendGetRequest(QUrl("https://www.newbiecontest.org/epreuves/prog/prog3_1.php"));

    // Root-me Captcha
    ht.sendGetRequest(QUrl("http://challenge01.root-me.org/programmation/ch8/"));

    return a.exec();
}
