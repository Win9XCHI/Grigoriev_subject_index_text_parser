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
};

#endif // CATEGORY_H
