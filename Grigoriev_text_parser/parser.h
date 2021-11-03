#ifndef PARSER_H
#define PARSER_H
#include <list>
#include "reader.h"
#include "classes/work.h"

class Parser
{
public:
    Reader Object_reader;
    std::list<Work*> Works;
    QString buffer;

    wchar_t CurrentSymbol;
    int NumberOfWork;
    QString CurrentPageNumber;
    QString CurrentProvince;
    //limits

    Parser();
    void ClearData();

    bool MainState();
    Work& WorkState();
    QString FirstPageNumberState();
    QString ProvinceState();
    QString NumberState(QString& numberInBook);
    QString NameState(std::list<Account_unit*>& units);
    QString NumberPageState();
    QString CityState();
    QString YearState();
    QString DegreeState();
    QString NavigationState();
    QString PersonState();

    Content& ContentState();
    QString CategoryState();
    QString PagesGraphsState();

    bool CheckEmpty(QString& str);
};

#endif // PARSER_H
