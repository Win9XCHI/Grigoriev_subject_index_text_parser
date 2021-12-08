#ifndef CONTENT_H
#define CONTENT_H
#include <list>
#include "category.h"
#include "pages.h"
#include "graphs.h"

class Content
{
public:
    std::list<Category*> Object_category;
    std::list<Pages*> Object_pages;
    std::list<Graphs*> Object_graphs;

    Content();
};

#endif // CONTENT_H
