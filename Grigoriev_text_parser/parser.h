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

    void MainState();
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
};

#endif // PARSER_H
