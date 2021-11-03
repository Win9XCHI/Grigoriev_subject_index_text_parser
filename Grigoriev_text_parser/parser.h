#ifndef PARSER_H
#define PARSER_H
#include "reader.h"
#include "classes/work.h"

class Parser
{
public:
    Reader Object_reader;
    Work Works;
    QString buffer;

    Parser();

    bool MainState();
    void WorkState();
    void ProvinceState();
    void PoinerState();
    void PersonState();
    void PagesState();
    void NavigationState();
    void GraphsState();
    void CountyState();
    void Account_unitState();
    void ContentState();

    void FirstPageNumber();
};

#endif // PARSER_H
