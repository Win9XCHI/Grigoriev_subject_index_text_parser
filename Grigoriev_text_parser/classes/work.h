#ifndef WORK_H
#define WORK_H
#include "province.h"
#include "content.h"
#include "navigation.h"
#include "person.h"
#include "pointer.h"
#include "account_unit.h"

class Work
{
public:
    Province* Object_province;
    Content* Object_content;
    Navigation* Object_navigation;
    Person* Object_person;
    Pointer* Object_pointer;
    Account_unit* Object_account_unit;


    Work();
    ~Work();
};

#endif // WORK_H
