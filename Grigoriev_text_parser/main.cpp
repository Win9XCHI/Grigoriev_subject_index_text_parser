#include <QCoreApplication>
#include "unparser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL, "");

    UnParser object_unparser;
    Parser* object_parser = (Parser*)&object_unparser;
    if (object_parser->MainState())
    {
        object_unparser.MainState();
    }

    return 0;
}
