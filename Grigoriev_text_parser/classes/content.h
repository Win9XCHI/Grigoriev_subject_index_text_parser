#ifndef CONTENT_H
#define CONTENT_H
#include "category.h"
#include "pages.h"
#include "graphs.h"

class Content
{
public:
    Category* Object_category;
    Pages* Object_pages;
    Graphs* Object_graphs;

    Content();
};

#endif // CONTENT_H
