#include <QCoreApplication>

#include "http.h"

#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Http ht(nullptr);

    // Prog1
    // ht.sendRequest(QUrl("https://www.newbiecontest.org/epreuves/prog/prog1.php"));

    // Prog3
    ht.sendRequest(QUrl("https://www.newbiecontest.org/epreuves/prog/prog3_1.php"));
    return a.exec();
}
