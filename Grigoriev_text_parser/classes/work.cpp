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

    if (Object_content)
    {
        delete Object_content;
    }

    if (Object_navigation)
    {
        delete Object_navigation;
    }

    if (Object_person)
    {
        delete Object_person;
    }

    if (Object_pointer)
    {
        delete Object_pointer;
    }

    if (Object_account_unit)
    {
        delete Object_account_unit;
    }
}
