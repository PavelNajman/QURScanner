#include <QtWidgets>

#include "urscanner.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    urscanner::UrScanner scanner;
    scanner.show();

    return app.exec();
};
