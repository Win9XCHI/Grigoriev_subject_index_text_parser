#ifndef ASSEMBLY_H
#define ASSEMBLY_H
#include <list>
#include "province.h"

class Assembly
{
public:
    QString Name;
    std::list<int> numbers;

    Assembly();
};

#endif // ASSEMBLY_H
