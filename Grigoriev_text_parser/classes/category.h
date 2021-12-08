#ifndef CATEGORY_H
#define CATEGORY_H
#include <list>
#include <qstring.h>
#include "pages.h"

class Category
{
public:
    QString Number;

    std::list<Pages*> object_pages;

    Category();

    unsigned long long CountOfPage();
    unsigned long long CountOfGraphs();
};

#endif // CATEGORY_H
