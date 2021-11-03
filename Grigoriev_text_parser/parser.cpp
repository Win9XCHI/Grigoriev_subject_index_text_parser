#include "parser.h"

Parser::Parser() : NumberOfWork(0)
{

}

void Parser::ClearData()
{
    for (Work* item : Works)
    {
        delete item;
    }
}

bool Parser::CheckEmpty(QString& str)
{
    CurrentSymbol = Object_reader.ReadChar();

    if (CurrentSymbol == '#')
    {
        return true;
    }

    str += CurrentSymbol;
    return false;
}

bool Parser::MainState()
{
    try {
        CurrentPageNumber = FirstPageNumberState();

        CurrentProvince = ProvinceState();

        while (!Object_reader.IsEOF())
        {
            Works.push_back(&WorkState());
            //NumberPageState()
        }

    } catch (bool flag) {
        Object_reader.Error();
        ClearData();
        return false;

    } catch (QString str) {
        Object_reader.Error(str);
        ClearData();
        return false;
    }

    return true;
}

QString Parser::FirstPageNumberState()
{
    QString number;
    CurrentSymbol = Object_reader.ReadChar();

    if (CurrentSymbol != '-')
    {
        throw "";
    }

    //if(r = getSometimng() > x) { }
    while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
    {
        number += CurrentSymbol;
    }

    if (CurrentSymbol != '-')
    {
        throw "";
    }

    bool flag = false;
    for (unsigned int i = 0; i < 200; i++)
    {
        CurrentSymbol = Object_reader.ReadChar(false);
        if (CurrentSymbol == '\0')
        {
            flag = true;
            break;
        }
    }

    if (!flag)
    {
        throw "";
    }

    return number;
}

QString Parser::ProvinceState()
{
    QString province;
    QString number;

    while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
    {
        number += CurrentSymbol;
    }

    if (CurrentSymbol != '.')
    {
        throw "";
    }

    if (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != ' ')
    {
        throw "";
    }

    while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != '.')
    {
        province += CurrentSymbol;
    }

    bool flag = false;
    for (unsigned int i = 0; i < 200; i++)
    {
        CurrentSymbol = Object_reader.ReadChar(false);
        if (CurrentSymbol == '\0')
        {
            flag = true;
            break;
        }
    }

    if (!flag)
    {
        throw "";
    }

    return province;
}

Work& Parser::WorkState()
{
    NumberOfWork++;
    Work* NewWork = new Work;

    QString numberInBook;
    QString number = NumberState(numberInBook);

    std::list<Account_unit*> units;
    QString name = NameState(units);

    QString city = CityState();
    QString year = YearState();
    QString degree = DegreeState();
    QString navigation = NavigationState();
    QString persons = PersonState();

    ContentState();


    return *NewWork;
}

QString Parser::NumberPageState()
{

}

QString Parser::NumberState(QString& numberInBook)
{
    QString number;

    while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
    {
        numberInBook += CurrentSymbol;
    }

    if (CurrentSymbol != '/')
    {
        throw "";
    }

    while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
    {
        number += CurrentSymbol;
    }

    if (CurrentSymbol != ' ')
    {
        throw "";
    }

    return number;
}

QString Parser::NameState(std::list<Account_unit*>& units)
{
    QString GeneralName;

   while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != '.')
   {
       if (CurrentSymbol == '&')
       {
            CurrentSymbol = '.';
       }

       GeneralName += CurrentSymbol;
   }

    while (true)
    {
        Account_unit* unit = new Account_unit;
        QString type;
        QString number;
        QString name;

        buffer = "";
        CurrentSymbol = Object_reader.ReadChar();

        if (CurrentSymbol != L'Т' && CurrentSymbol != L'Ч' && CurrentSymbol != L'В')
        {
            buffer += CurrentSymbol;
            break;
        }

        type += CurrentSymbol;

        if (CurrentSymbol == L'В')
        {
            CurrentSymbol = Object_reader.ReadChar();

            if (CurrentSymbol != L'ы')
            {
                buffer += CurrentSymbol;
                break;
            }

            type += CurrentSymbol;
            CurrentSymbol = Object_reader.ReadChar();

            if (CurrentSymbol != L'п')
            {
                buffer += CurrentSymbol;
                break;
            }

            type += CurrentSymbol;
        }

        unit->Type = type;
        CurrentSymbol = Object_reader.ReadChar();

        if (CurrentSymbol != '.')
        {
            buffer += CurrentSymbol;
            break;
        }

        CurrentSymbol = Object_reader.ReadChar();

        if (CurrentSymbol == ' ')
        {
            CurrentSymbol = Object_reader.ReadChar();
        }

        while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
        {
            number += CurrentSymbol;
        }

        unit->Number = number.toInt();

        if (CurrentSymbol != '.')
        {
            buffer += CurrentSymbol;
            break;
        }

        CurrentSymbol = Object_reader.ReadChar();

        if (CurrentSymbol == ' ')
        {
            CurrentSymbol = Object_reader.ReadChar();
        }

        if (CurrentSymbol == '#')
        {
            units.push_back(unit);
            return GeneralName;
        }

        while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != '.')
        {
            if (CurrentSymbol == '&')
            {
                 CurrentSymbol = '.';
            }

            name += CurrentSymbol;
        }

        unit->Name = name;
        units.push_back(unit);
    }

    return GeneralName;
}

QString Parser::CityState()
{
    QString city = buffer;

    if(CheckEmpty(city))
    {
        return "";
    }

    while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != ',')
    {
        city += CurrentSymbol;
    }

    CurrentSymbol = Object_reader.ReadChar();

    if (CurrentSymbol != ' ')
    {
        throw "";
    }

    return city;
}

QString Parser::YearState()
{
    QString year;

    if(CheckEmpty(year))
    {
        return "";
    }

    while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
    {
        year += CurrentSymbol;
    }

    if (CurrentSymbol != ' ')
    {
        throw "";
    }

    CurrentSymbol = Object_reader.ReadChar();

    if (CurrentSymbol != L'г')
    {
        throw "";
    }

    CurrentSymbol = Object_reader.ReadChar();

    if (CurrentSymbol != '.')
    {
        throw "";
    }

    CurrentSymbol = Object_reader.ReadChar();

    if (CurrentSymbol != ' ')
    {
        throw "";
    }

    return year;
}

QString Parser::DegreeState()
{
    QString degree;

    if (CheckEmpty(degree))
    {
        return "";
    }

    while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
    {
        degree += CurrentSymbol;
    }

    if (CurrentSymbol != L'°')
    {

    }

    CurrentSymbol = Object_reader.ReadChar();

    if (CurrentSymbol != ' ')
    {
        throw "";
    }

    return degree;
}

QString Parser::NavigationState()
{

}

QString Parser::PersonState()
{

}

Content& Parser::ContentState()
{
    CategoryState();
    PagesGraphsState();
}

QString Parser::CategoryState()
{

}

QString Parser::PagesGraphsState()
{

}


