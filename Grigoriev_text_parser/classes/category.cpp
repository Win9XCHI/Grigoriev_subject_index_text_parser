#include "category.h"

Category::Category()
{

}

unsigned long long Category::CountOfPage()
{
    return object_pages.size() - CountOfGraphs();
}

unsigned long long Category::CountOfGraphs()
{
    unsigned long long count = 0;

    for (auto& item : object_pages)
    {
        if (item->graphs)
        {
            count++;
        }
    }

    return count;
}
