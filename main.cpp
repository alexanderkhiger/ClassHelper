
#include <QApplication>

#include "authview.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif
    QApplication app(argc, argv);
    AuthView window;
    window.show();
    return app.exec();
}
