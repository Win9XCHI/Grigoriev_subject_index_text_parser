#ifndef PARSER_H
#define PARSER_H
#include <list>
#include "writer.h"
#include "classes/work.h"
#include "constants.h"

class Parser
{
public:
    Reader* Object_reader;
    Writer Object_writer;
    std::list<Work*> Works;
    QString buffer;

    wchar_t CurrentSymbol;
    int NumberOfWork;
    QString CurrentPageNumber;
    QString CurrentProvince;
    //limits

    Parser();
    void ClearData();

    virtual bool MainState();
    Work& WorkState();
    QString FirstPageNumberState();
    QString ProvinceState();
    QString NumberState(QString& numberInBook);
    QString NameState(std::list<Account_unit*>& units);
    QString NumberPageState();
    QString CityState();
    QString YearState();
    QString DegreeState();
    void NavigationState(std::list<Navigation*>& Nav);
    void PersonState(std::list<Person*>& Persons);

    Content* ContentState();
    void CategoryState(std::list<Category*>& category);
    void PagesGraphsState(std::list<Pages*>& page, std::list<Graphs*>& graph, Category& category);

    bool CheckEmpty(QString& str);
    void FillAllCategories(std::list<Category*>& category);
};

#endif // PARSER_H
