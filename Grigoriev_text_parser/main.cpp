#include <QCoreApplication>
#include "parser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL, "");

    Parser object_parser;
    object_parser.MainState();

    return a.exec();
}
