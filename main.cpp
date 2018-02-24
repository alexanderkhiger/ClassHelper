
#include <QApplication>

#include "authview.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QApplication app(argc, argv);
    AuthView window;
    window.show();
    return app.exec();
}
