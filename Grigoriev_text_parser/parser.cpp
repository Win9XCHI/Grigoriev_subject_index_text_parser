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
            NumberPageState();
        }

    } catch (bool flag) {
        Object_reader.Error();
        ClearData();
        return false;

    } catch (QString str) {
        Object_reader.Error(str);
        ClearData();
        return false;
    } catch (const char* str) {
        Object_reader.Error(str);
        ClearData();
        return false;
    }

    return true;
}

QString Parser::FirstPageNumberState()
{
    QString number;
    int limit = 10;
    CurrentSymbol = Object_reader.ReadChar();

    if (CurrentSymbol != '-')
    {
        throw "FirstPageNumberState 1";
    }

    //if(r = getSometimng() > x) { }
    while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
    {
        number += CurrentSymbol;

        if (limit == 0)
        {
            throw "FirstPageNumberState 2. Out of limit";
        }
        limit--;
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
        throw "FirstPageNumberState 3";
    }

    return number;
}

QString Parser::ProvinceState()
{
    QString province;
    QString number;
    int limit = 100;

    while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
    {
        number += CurrentSymbol;

        if (limit == 0)
        {
            throw "ProvinceState 1. Out of limit";
        }
        limit--;
    }

    if (CurrentSymbol != '.')
    {
        throw "ProvinceState 2";
    }

    if (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != ' ')
    {
        throw "ProvinceState 3";
    }

    while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != '.')
    {
        province += CurrentSymbol;

        if (limit == 0)
        {
            throw "ProvinceState 4. Out of limit";
        }
        limit--;
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
        throw "ProvinceState 5";
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

    std::list<Navigation*> Nav;
    NavigationState(Nav);

    std::list<Person*> Persons;
    PersonState(Persons);

    while (buffer != '\0' || (CurrentSymbol = Object_reader.ReadChar() != '\0')) {}

    *NewWork->Object_content = ContentState();

    return *NewWork;
}

QString Parser::NumberPageState()
{
    QString number;
    int limit = 100;

    while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol == '\0')
    {
        if (limit == 0)
        {
            throw "NumberPageState 1. Out of limit";
        }
        limit--;
    }

    CurrentSymbol = Object_reader.ReadChar();

    if (CurrentSymbol != '-')
    {
        buffer = CurrentSymbol;
        return "";
    }

    limit = 100;
    while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
    {
        number += CurrentSymbol;

        if (limit == 0)
        {
            throw "NumberPageState 2. Out of limit";
        }
        limit--;
    }

    if (CurrentSymbol != '-')
    {
        throw "NumberPageState 3";
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
        throw "NumberPageState 4";
    }

    return number;
}

QString Parser::NumberState(QString& numberInBook)
{
    QString number;
    int limit = 100;

    if (!buffer.isEmpty())
    {
        numberInBook = buffer;
        buffer = "";
    }

    while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
    {
        numberInBook += CurrentSymbol;

        if (limit == 0)
        {
            throw "NumberState 1. Out of limit";
        }
        limit--;
    }

    if (CurrentSymbol != '/')
    {
        throw "NumberState 2";
    }

    limit = 100;
    while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
    {
        number += CurrentSymbol;

        if (limit == 0)
        {
            throw "NumberState 3. Out of limit";
        }
        limit--;
    }

    if (CurrentSymbol != ' ')
    {
        throw "NumberState 4";
    }

    return number;
}

QString Parser::NameState(std::list<Account_unit*>& units)
{
    QString GeneralName;
    int limit = 100;

   while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != '.')
   {
       if (CurrentSymbol == '&')
       {
            CurrentSymbol = '.';
       }

       GeneralName += CurrentSymbol;

       if (limit == 0)
       {
           throw "NameState 1. Out of limit";
       }
       limit--;
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

        limit = 100;
        while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
        {
            number += CurrentSymbol;

            if (limit == 0)
            {
                throw "NameState 2. Out of limit";
            }
            limit--;
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

        limit = 100;
        while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != '.')
        {
            if (CurrentSymbol == '&')
            {
                 CurrentSymbol = '.';
            }

            name += CurrentSymbol;

            if (limit == 0)
            {
                throw "NameState 3. Out of limit";
            }
            limit--;
        }

        unit->Name = name;
        units.push_back(unit);
    }

    return GeneralName;
}

QString Parser::CityState()
{
    QString city = buffer;
    int limit = 100;

    if(CheckEmpty(city))
    {
        return "";
    }

    while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != ',')
    {
        city += CurrentSymbol;

        if (limit == 0)
        {
            throw "CityState 1. Out of limit";
        }
        limit--;
    }

    CurrentSymbol = Object_reader.ReadChar();

    if (CurrentSymbol != ' ')
    {
        throw "CityState 2";
    }

    return city;
}

QString Parser::YearState()
{
    QString year;
    int limit = 100;

    if(CheckEmpty(year))
    {
        return "";
    }

    while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
    {
        year += CurrentSymbol;

        if (limit == 0)
        {
            throw "YearState 1. Out of limit";
        }
        limit--;
    }

    if (CurrentSymbol != ' ')
    {
        throw "YearState 2";
    }

    CurrentSymbol = Object_reader.ReadChar();

    if (CurrentSymbol != L'г')
    {
        throw "YearState 3";
    }

    CurrentSymbol = Object_reader.ReadChar();

    if (CurrentSymbol != '.')
    {
        throw "YearState 4";
    }

    CurrentSymbol = Object_reader.ReadChar();

    if (CurrentSymbol != ' ')
    {
        throw "YearState 5";
    }

    return year;
}

QString Parser::DegreeState()
{
    QString degree;
    int limit = 100;

    if (CheckEmpty(degree))
    {
        return "";
    }

    while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
    {
        degree += CurrentSymbol;

        if (limit == 0)
        {
            throw "DegreeState 1. Out of limit";
        }
        limit--;
    }

    if (CurrentSymbol != L'°')
    {
        throw "DegreeState 2";
    }

    CurrentSymbol = Object_reader.ReadChar();

    if (CurrentSymbol != ' ')
    {
        throw "DegreeState 3";
    }

    return degree;
}

bool string_isdigit(QString str)
{
    const char* string = qPrintable(str);
    for (unsigned int i = 0; i < str.size(); i++)
    {
        if (!isdigit(string[i]))
        {
            return false;
        }
    }

    return true;
}

void Parser::NavigationState(std::list<Navigation*>& Nav)
{
    QString buffer_str;
    Navigation* buffer_nav;
    bool next = true;
    bool switcher = true;
    int limit = 100;

    if (CheckEmpty(buffer_str))
    {
        return;
    }

    while (next)
    {
        next = false;

        limit = 100;
        while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != '+' || CurrentSymbol != '=' || CurrentSymbol != ' ' || CurrentSymbol != '/')
        {
            buffer_str += CurrentSymbol;

            if (limit == 0)
            {
                throw "NavigationState 1. Out of limit";
            }
            limit--;
        }

        if (switcher)
        {
            buffer_nav = new Navigation;

            if (string_isdigit(buffer_str))
            {
                buffer_nav->Arabic = buffer_str.toInt();
            } else
            {
                buffer_nav->Rome = buffer_str;
            }

        } else
        {
            //if (buffer_nav)
            //{
                buffer_nav->Number = buffer_str;
                Nav.push_back(buffer_nav);
            //}
        }

        if (CurrentSymbol == '+' || CurrentSymbol == '=')
        {
            next = true;
            switcher = true;
        }

        if (CurrentSymbol == '/')
        {
            switcher = false;
        }

        if (CurrentSymbol == ' ')
        {
            CurrentSymbol = Object_reader.ReadChar();

            if (CurrentSymbol != L'и' || CurrentSymbol != L'с')
            {
                throw "NavigationState 2";
            }

            if (CurrentSymbol == L'и')
            {
                buffer_nav->NumberInNextNav = true;
                CurrentSymbol = Object_reader.ReadChar();

                if (CurrentSymbol != ' ')
                {
                    throw "NavigationState 3";
                }

                switcher = true;

            } else
            {
                CurrentSymbol = Object_reader.ReadChar();

                if (CurrentSymbol != L'т')
                {
                    throw "NavigationState 4";
                }

                CurrentSymbol = Object_reader.ReadChar();

                if (CurrentSymbol != L'р')
                {
                    throw "NavigationState 5";
                }

                CurrentSymbol = Object_reader.ReadChar();

                if (CurrentSymbol != '.')
                {
                    throw "NavigationState 6";
                }

                CurrentSymbol = Object_reader.ReadChar();

                if (CurrentSymbol != ' ')
                {
                    throw "NavigationState 7";
                }

                CurrentSymbol = Object_reader.ReadChar();

                if (CurrentSymbol != L'и')
                {
                    return;
                }

                CurrentSymbol = Object_reader.ReadChar();

                if (CurrentSymbol != ' ')
                {
                    return;
                }

                buffer_str = "";
                limit = 100;
                while (CurrentSymbol = Object_reader.ReadChar() && !isupper(CurrentSymbol) && CurrentSymbol != '#')
                {
                    buffer_str += CurrentSymbol;

                    if (limit == 0)
                    {
                        throw "NavigationState 8. Out of limit";
                    }
                    limit--;
                }

                buffer_nav->AddInfo = buffer_str.left(buffer_str.size() - 1);
                Nav.push_back(buffer_nav);
                buffer = CurrentSymbol;
                return;
            }

        }

        buffer_str = "";
    }
}

void Parser::PersonState(std::list<Person*>& Persons)
{
    QString buffer_str = buffer;
    QString CurrentRole;
    int limit = 100;

    if (CheckEmpty(buffer))
    {
        return;
    }

    if (buffer == '?')
    {
        while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != '\0')
        {
            buffer_str += CurrentSymbol;

            if (limit == 0)
            {
                throw "PersonStat 1. Out of limit";
            }
            limit--;
        }

        Person* pers = new Person;
        pers->Name = buffer_str;
        Persons.push_back(pers);
        buffer = CurrentSymbol;
        return;
    }

    while (true)
    {
        limit = 100;
        while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != ':')
        {
            buffer_str += CurrentSymbol;

            if (limit == 0)
            {
                throw "PersonStat 2. Out of limit";
            }
            limit--;
        }

        CurrentRole = buffer_str;
        buffer_str = "";

        CurrentSymbol = Object_reader.ReadChar();

        if (CurrentSymbol != ' ')
        {
            throw "PersonStat 3";
        }

        while (true)
        {
            limit = 100;
            while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != ',' && CurrentSymbol != '.' && CurrentSymbol != ';')
            {
                if (CurrentSymbol == '&')
                {
                    CurrentSymbol = '.';
                }
                buffer_str += CurrentSymbol;

                if (limit == 0)
                {
                    throw "PersonStat 4. Out of limit";
                }
                limit--;
            }

            Person* pers = new Person;
            pers->Name = buffer_str;
            pers->Role = CurrentRole;
            Persons.push_back(pers);

            if (CurrentSymbol == ';')
            {
                break;
            }

            if (CurrentSymbol == ',')
            {
                CurrentSymbol = Object_reader.ReadChar();

                if (CurrentSymbol != ' ')
                {
                    throw "PersonStat 5";
                }
            }

            if (CurrentSymbol == '.')
            {
                return;
            }
        }
    }
}

Content& Parser::ContentState()
{
    Content object_content;

    while (CurrentSymbol != '.')
    {
        CategoryState(object_content.Object_category);

        if (CurrentSymbol == '`')
        {
            continue;
        }

        PagesGraphsState(object_content.Object_pages, object_content.Object_graphs);
    }

    return object_content;
}

void Parser::CategoryState(std::list<Category*> category)
{
    QString buffer_str;
    int limit = 100;

    while (true)
    {
        buffer_str = "";
        Category* cat = new Category;

        limit = 100;
        while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
        {
            buffer_str += CurrentSymbol;

            if (limit == 0)
            {
                throw "CategoryState 1. Out of limit";
            }
            limit--;
        }

        cat->Number = buffer_str.toInt();
        category.push_back(cat);

        if (CurrentSymbol == ';')
        {
            CurrentSymbol = '`';
            return;
        }

        if (CurrentSymbol == ',')
        {
            continue;
        }

        if (CurrentSymbol == ' ')
        {
            CurrentSymbol = Object_reader.ReadChar();
            if (CurrentSymbol == L'и')
            {
                CurrentSymbol = Object_reader.ReadChar();
                if (CurrentSymbol != ' ')
                {
                    throw "CategoryState 2";
                }

                continue;
            }
        }

        return;
    }
}

void Parser::PagesGraphsState(std::list<Pages*> page, std::list<Graphs*> graph)
{
    QString buffer_str;
    Pages* pg = nullptr;
    bool switcher = false;
    int limit = 100;

    while (true)
    {
        buffer_str = "";

        limit = 100;
        while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != ',' && CurrentSymbol != '.' && CurrentSymbol != ';' && CurrentSymbol != ' ' && CurrentSymbol != '-')
        {
            buffer_str += CurrentSymbol;

            if (limit == 0)
            {
                throw "PagesGraphsState 1. Out of limit";
            }
            limit--;
        }

        if (!switcher)
        {
            pg = new Pages;
            pg->Number_1 = buffer_str;
            page.push_back(pg);
        } else
        {
            int i = 0;
            for (Pages* item : page)
            {
                if (page.size() - 1 == i)
                {
                    pg = item;
                }
                i++;
            }
            pg->Number_2 = buffer_str;
            switcher = false;
        }

         //1
        if (CurrentSymbol == ',')
        {
            return;
        }

        if (CurrentSymbol == ' ')
        {
            CurrentSymbol = Object_reader.ReadChar();

            if (CurrentSymbol == L'и')
            {
                CurrentSymbol = Object_reader.ReadChar();

                if (CurrentSymbol != ' ')
                {
                    throw "PagesGraphsState 2";
                }
                continue;
            }

            if (CurrentSymbol != L'г')
            {
                throw "PagesGraphsState 3";
            }

            if (CurrentSymbol != L'р')
            {
                throw "PagesGraphsState 4";
            }

            int i = 0;
            for (Pages* item : page)
            {
                if (page.size() - 1 == i)
                {
                    pg = item;
                }
                i++;
            }
            Graphs* gr = new Graphs;
            gr->Number_1 = pg->Number_1;
            gr->Number_2 = pg->Number_2;

            CurrentSymbol = Object_reader.ReadChar();

            if (CurrentSymbol == '.')
            {
                CurrentSymbol = Object_reader.ReadChar();

                //1
                if (CurrentSymbol == ';')
                {
                    graph.push_back(gr);
                    page.pop_back();
                    continue;
                }

                if (CurrentSymbol == ' ')
                {
                    CurrentSymbol = Object_reader.ReadChar();

                    if (CurrentSymbol == L'и')
                    {
                        CurrentSymbol = Object_reader.ReadChar();

                        if (CurrentSymbol != ' ')
                        {
                            throw "PagesGraphsState 5";
                        }
                        graph.push_back(gr);
                        page.pop_back();
                        continue;
                    }
                }
            }

            if (CurrentSymbol == ' ')
            {
                CurrentSymbol = Object_reader.ReadChar();

                if (CurrentSymbol != '/')
                {
                    throw "PagesGraphsState 6";
                }
            }

            if (CurrentSymbol == '/')
            {
                buffer_str = "";
                limit = 100;
                while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != '.')
                {
                    if (CurrentSymbol == ' ')
                    {
                        continue;
                    }

                    buffer_str += CurrentSymbol;

                    if (limit == 0)
                    {
                        throw "PagesGraphsState 7. Out of limit";
                    }
                    limit--;
                }

                gr->Table_g = buffer_str;
                graph.push_back(gr);
                page.pop_back();

                 //1
                if (CurrentSymbol == '.')
                {
                    CurrentSymbol = Object_reader.ReadChar();

                    if (CurrentSymbol == ';')
                    {
                        continue;
                    }

                    if (CurrentSymbol == ' ')
                    {
                        CurrentSymbol = Object_reader.ReadChar();

                        if (CurrentSymbol == L'и')
                        {
                            CurrentSymbol = Object_reader.ReadChar();

                            if (CurrentSymbol != ' ')
                            {
                                throw "PagesGraphsState 8";
                            }
                            continue;
                        }
                    }
                }
            }
        }

        if (CurrentSymbol == ';')
        {
            continue;
        }

        if (CurrentSymbol == '.')
        {
            return;
        }

        if (CurrentSymbol == '-')
        {
            switcher = true;
        }
    }

}


