
#include <QApplication>

#include "authview.h"
// #include "universityview.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    AuthView window;
//    UniversityView window;
    window.show();
    return app.exec();
}
