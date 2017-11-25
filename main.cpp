
#include <QApplication>

#include "auth.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Auth window;
    window.show();
    return app.exec();
}
