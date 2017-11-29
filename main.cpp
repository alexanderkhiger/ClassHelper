
#include <QApplication>

#include "authview.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    AuthView window;
    window.show();
    return app.exec();
}
