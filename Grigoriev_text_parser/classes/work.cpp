#include "work.h"

Work::Work()
{

}

Work::~Work()
{
    if (Object_province)
    {
        delete Object_province;
    }
    if (Object_content.size() > 0)
    {
        for (auto& item : Object_content)
        {
            delete item;
        }
    }
    if (Object_navigation.size() > 0)
    {
        for (auto& item : Object_navigation)
        {
            delete item;
        }
    }
    if (Object_person.size() > 0)
    {
        for (auto& item : Object_person)
        {
            delete item;
        }
    }
    if (Object_pointer)
    {
        delete Object_pointer;
    }
    if (Object_account_unit.size() > 0)
    {
        for (auto& item : Object_account_unit)
        {
            delete item;
        }
    }
}
