#ifndef UNPARSER_H
#define UNPARSER_H
#include <parser.h>

class UnParser : public Parser
{
public:
    UnParser();

    bool MainState();
    void OUseDB();
    void OProvinces();
    void OAssemblies();
    void OPersons(std::list<Person*> persons);
    void OWork(Work* work);
    void OPointer(Pointer* point);
    void ONavigation(std::list<Navigation*> nav);
    void OUnits(std::list<Account_unit*> unit);
    void OCounties();
    void OC_W();
    void OP_W(Person* pers);
    void OContent_Pages_Graphs(std::list<Content*> content);
};

#endif // UNPARSER_H
