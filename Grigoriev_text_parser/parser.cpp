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

    std::list<Navigation*> Nav;
    NavigationState(Nav);

    std::list<Person*> Persons;
    PersonState(Persons);

    while (buffer != '\0' || (CurrentSymbol = Object_reader.ReadChar() != '\0')) {}

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

    if (CheckEmpty(buffer_str))
    {
        return;
    }

    while (next)
    {
        next = false;

        while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != '+' || CurrentSymbol != '=' || CurrentSymbol != ' ' || CurrentSymbol != '/')
        {
            buffer_str += CurrentSymbol;
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
                throw "";
            }

            if (CurrentSymbol == L'и')
            {
                buffer_nav->NumberInNextNav = true;
                CurrentSymbol = Object_reader.ReadChar();

                if (CurrentSymbol != ' ')
                {
                    throw "";
                }

                switcher = true;

            } else
            {
                CurrentSymbol = Object_reader.ReadChar();

                if (CurrentSymbol != L'т')
                {
                    throw "";
                }

                CurrentSymbol = Object_reader.ReadChar();

                if (CurrentSymbol != L'р')
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
                while (CurrentSymbol = Object_reader.ReadChar() && !isupper(CurrentSymbol) && CurrentSymbol != '#')
                {
                    buffer_str += CurrentSymbol;
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

    if (CheckEmpty(buffer))
    {
        return;
    }

    if (buffer == '?')
    {
        while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != '\0')
        {
            buffer_str += CurrentSymbol;
        }

        Person* pers = new Person;
        pers->Name = buffer_str;
        Persons.push_back(pers);
        buffer = CurrentSymbol;
        return;
    }

    while (true)
    {
        while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != ':')
        {
            buffer_str += CurrentSymbol;
        }

        CurrentRole = buffer_str;
        buffer_str = "";

        CurrentSymbol = Object_reader.ReadChar();

        if (CurrentSymbol != ' ')
        {
            throw "";
        }

        while (true)
        {
            while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != ',' && CurrentSymbol != '.' && CurrentSymbol != ';')
            {
                if (CurrentSymbol == '&')
                {
                    CurrentSymbol = '.';
                }
                buffer_str += CurrentSymbol;
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
                    throw "";
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

    while (true)
    {
        buffer_str = "";
        Category* cat = new Category;

        while (CurrentSymbol = Object_reader.ReadChar() && isdigit(CurrentSymbol))
        {
            buffer_str += CurrentSymbol;
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
                    throw "";
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

    while (true)
    {
        buffer_str = "";

        while (CurrentSymbol = Object_reader.ReadChar() && CurrentSymbol != ',' && CurrentSymbol != '.' && CurrentSymbol != ';' && CurrentSymbol != ' ' && CurrentSymbol != '-')
        {
            buffer_str += CurrentSymbol;
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
                    throw "";
                }
                continue;
            }

            if (CurrentSymbol != L'г')
            {
                throw "";
            }

            if (CurrentSymbol != L'р')
            {
                throw "";
            }

            //...graph
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


