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
    QString province;
    std::list<Content*> Object_content;
    std::list<Navigation*> Object_navigation;
    std::list<Person*> Object_person;
    Pointer* Object_pointer;
    std::list<Account_unit*> Object_account_unit;
    QString name;
    QString city;
    QString year;
    QString degree;

    int NumberInBook;

    Work();
    ~Work();
};

#endif // WORK_H
