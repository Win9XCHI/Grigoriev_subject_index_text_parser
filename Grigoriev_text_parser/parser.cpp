#include "parser.h"

Parser::Parser() : NumberOfWork(0)
{
    Object_reader = (Reader*)&Object_writer;
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
    CurrentSymbol = Object_reader->ReadChar();

    if (CurrentSymbol == '#')
    {
        return true;
    }

    str += CurrentSymbol;
    return false;
}

void Parser::AddAssembly(QString Name, std::list<int> Numbers, int From, int To)
{
    Assembly ass;
    ass.Name = Name;

    for (int& item : Numbers)
    {
        ass.numbers.push_back(item);
    }

    for (int i = From; i <= To; i++)
    {
        ass.numbers.push_back(i);
    }

    Assemblies.push_back(ass);
}

void Parser::FillAssemblies()
{
    AddAssembly("Материалы для оценки земель", std::list<int>(77, 78), 1126, 1132);

    AddAssembly("Сборник статистич. свед.", std::list<int>(), 1121, 1122);

    AddAssembly("Статистико-экономич. таблицы", std::list<int>(), 1123, 1124);

    AddAssembly("Список населенных мест", std::list<int>(), 1134, 1141);

    AddAssembly("Урожай хлебов и трав", std::list<int>(), 1143, 1144);

    AddAssembly("Сведения об урожае хлебов и трав", std::list<int>(), 1145, 1146);
}

bool Parser::MainState()
{
    try {
        FillAssemblies();

        CurrentPageNumber = FirstPageNumberState();

        CurrentProvince = ProvinceState();

        while (!Object_reader->IsEOF())
        {
            Works.push_back(&WorkState());
            NumberPageState();
        }

    } catch (bool flag) {
        Object_writer.Error();
        ClearData();
        return false;

    } catch (QString str) {
        if (str != "The end")
        {
            Object_writer.Error(str);
            ClearData();
            return false;
        }
    } catch (const char* str) {
        QString f = str;
        if (f != "The end")
        {
            Object_writer.Error(str);
            ClearData();
            return false;
        }
    }

    Object_reader->CloseFile();

    return true;
}

QString Parser::FirstPageNumberState()
{
    QString number;
    int limit = 10;
    CurrentSymbol = Object_reader->ReadChar();

    if (CurrentSymbol != 8212)  //'-'
    {
        throw "FirstPageNumberState 1";
    }

    //if(r = getSometimng() > x) { }
    while (CurrentSymbol = Object_reader->ReadChar())
    {
        if (!iswdigit(CurrentSymbol))
        {
            break;
        }

        number += CurrentSymbol;

        if (limit == 0)
        {
            throw "FirstPageNumberState 2. Out of limit";
        }
        limit--;
    }

    if (CurrentSymbol != 8212)  //'-'
    {
        throw "FirstPageNumberState 3";
    }

    bool flag = false;
    for (unsigned int i = 0; i < 200; i++)
    {
        CurrentSymbol = Object_reader->ReadChar(false);
        if (CurrentSymbol == '\0')
        {
            flag = true;
            break;
        }
    }

    if (!flag)
    {
        throw "FirstPageNumberState 4";
    }

    return number;
}

QString Parser::ProvinceState()
{
    QString province;
    QString number;
    int limit = 100;

    while (CurrentSymbol = Object_reader->ReadChar())
    {
        if (!iswdigit(CurrentSymbol))
        {
            break;
        }

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

    CurrentSymbol = Object_reader->ReadChar();
    if (CurrentSymbol != ' ')
    {
        throw "ProvinceState 3";
    }

    limit = 100;
    while (CurrentSymbol = Object_reader->ReadChar())
    {
        if (CurrentSymbol == '.')
        {
            break;
        }

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
        CurrentSymbol = Object_reader->ReadChar(false);
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
    NewWork->province = CurrentProvince;

    QString numberInBook;
    QString number = NumberState(numberInBook);
    NewWork->NumberInBook = numberInBook.toInt();

    Pointer* pointer = new Pointer;
    pointer->Number = number.toInt();
    pointer->Page = CurrentPageNumber.toInt();
    pointer->Part = PART;
    NewWork->Object_pointer = pointer;

    std::list<Account_unit*> units;
    QString name = NameState(units);
    NewWork->name = name;
    NewWork->Object_account_unit = units;

    QString city = CityState();
    QString year = YearState();
    QString degree = DegreeState();

    NewWork->city = city;
    NewWork->year = year;
    NewWork->degree = degree;

    std::list<Navigation*> Nav;
    NavigationState(Nav);
    NewWork->Object_navigation = Nav;

    std::list<Person*> Persons;
    PersonState(Persons);
    NewWork->Object_person = Persons;

    if (!buffer.isEmpty())
    {
        throw "Buffer is not empty";
    }

    /*while (CurrentSymbol = Object_reader->ReadChar())
    {
        if (CurrentSymbol == '\0')
        {
            break;
        }
    }*/

    NewWork->Object_content.push_back(ContentState());

    return *NewWork;
}

QString Parser::NumberPageState()
{
    QString number;
    int limit = 100;

    CurrentSymbol = Object_reader->ReadChar();

    if (CurrentSymbol != 8212)
    {
        buffer = CurrentSymbol;
        return "";
    }

    limit = 100;
    while (CurrentSymbol = Object_reader->ReadChar())
    {
        if (!iswdigit(CurrentSymbol))
        {
            break;
        }

        number += CurrentSymbol;

        if (limit == 0)
        {
            throw "NumberPageState 2. Out of limit";
        }
        limit--;
    }

    if (CurrentSymbol != 8212)
    {
        throw "NumberPageState 3";
    }

    bool flag = false;
    for (unsigned int i = 0; i < 200; i++)
    {
        CurrentSymbol = Object_reader->ReadChar(false);
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

    while (CurrentSymbol = Object_reader->ReadChar())
    {
        if (!iswdigit(CurrentSymbol))
        {
            break;
        }

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
    while (CurrentSymbol = Object_reader->ReadChar())
    {
        if (!iswdigit(CurrentSymbol))
        {
            break;
        }

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
    int limit = 300;

   while (CurrentSymbol = Object_reader->ReadChar())
   {
       if (CurrentSymbol == '.')
       {
           break;
       }

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
        CurrentSymbol = Object_reader->ReadChar();

        if (CurrentSymbol != ' ')
        {
            if (unit)
            {
                delete unit;
                unit = nullptr;
            }
            throw "NameState. Out of limit";
        }

        CurrentSymbol = Object_reader->ReadChar();

        if (CurrentSymbol != L'Т' && CurrentSymbol != L'Ч' && CurrentSymbol != L'В')
        {
            buffer += CurrentSymbol;
            break;
        }

        type += CurrentSymbol;

        if (CurrentSymbol == L'В')
        {
            CurrentSymbol = Object_reader->ReadChar();

            if (CurrentSymbol != L'ы')
            {
                buffer += CurrentSymbol;
                break;
            }

            type += CurrentSymbol;
            CurrentSymbol = Object_reader->ReadChar();

            if (CurrentSymbol != L'п')
            {
                buffer += CurrentSymbol;
                break;
            }

            type += CurrentSymbol;
        }

        unit->Type = type;
        CurrentSymbol = Object_reader->ReadChar();

        if (CurrentSymbol != '.')
        {
            buffer += CurrentSymbol;
            break;
        }

        CurrentSymbol = Object_reader->ReadChar();

        if (CurrentSymbol == ' ')
        {
            CurrentSymbol = Object_reader->ReadChar();
        }

        limit = 100;
        while (CurrentSymbol = Object_reader->ReadChar())
        {
            if (!iswdigit(CurrentSymbol))
            {
                break;
            }

            number += CurrentSymbol;

            if (limit == 0)
            {
                if (unit)
                {
                    delete unit;
                    unit = nullptr;
                }
                throw "NameState 2. Out of limit";
            }
            limit--;
        }

        unit->Number = number;

        if (CurrentSymbol != '.')
        {
            buffer += CurrentSymbol;
            break;
        }

        CurrentSymbol = Object_reader->ReadChar();

        if (CurrentSymbol == ' ')
        {
            CurrentSymbol = Object_reader->ReadChar();
        }

        if (CurrentSymbol == '#')
        {
            units.push_back(unit);
            return GeneralName;
        }

        name = CurrentSymbol;

        limit = 100;
        while (CurrentSymbol = Object_reader->ReadChar())
        {
            if (CurrentSymbol == '.')
            {
                break;
            }

            if (CurrentSymbol == '&')
            {
                 CurrentSymbol = '.';
            }

            name += CurrentSymbol;

            if (limit == 0)
            {
                if (unit)
                {
                    delete unit;
                    unit = nullptr;
                }
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

    while (CurrentSymbol = Object_reader->ReadChar())
    {
        if (CurrentSymbol == ',')
        {
            break;
        }

        city += CurrentSymbol;

        if (limit == 0)
        {
            throw "CityState 1. Out of limit";
        }
        limit--;
    }

    CurrentSymbol = Object_reader->ReadChar();

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

    while (CurrentSymbol = Object_reader->ReadChar())
    {
        if (!iswdigit(CurrentSymbol))
        {
            break;
        }

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

    CurrentSymbol = Object_reader->ReadChar();

    if (CurrentSymbol != L'г')
    {
        throw "YearState 3";
    }

    CurrentSymbol = Object_reader->ReadChar();

    if (CurrentSymbol != '.')
    {
        throw "YearState 4";
    }

    CurrentSymbol = Object_reader->ReadChar();

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

    while (CurrentSymbol = Object_reader->ReadChar())
    {
        if (!iswdigit(CurrentSymbol))
        {
            break;
        }

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

    CurrentSymbol = Object_reader->ReadChar();

    if (CurrentSymbol != ',')
    {
        throw "DegreeState 3";
    }

    CurrentSymbol = Object_reader->ReadChar();

    if (CurrentSymbol != ' ')
    {
        throw "DegreeState 4";
    }

    return degree;
}

bool string_iswdigit(QString str)
{
    const wchar_t* string = str.toStdWString().c_str(); //qPrintable(str);
    for (unsigned int i = 0; i < str.size(); i++)
    {
        if (!iswdigit(string[i]))
        {
            return false;
        }
    }

    return true;
}

void Parser::NavigationState(std::list<Navigation*>& Nav)
{
    QString buffer_str;
    Navigation* buffer_nav = nullptr;
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
        while (CurrentSymbol = Object_reader->ReadChar())
        {
            if ( CurrentSymbol == '+' || CurrentSymbol == '=' || CurrentSymbol == ' ' || CurrentSymbol == '/')
            {
                break;
            }

            if (CurrentSymbol == '&')
            {
                buffer_str += '.';
                CurrentSymbol = ' ';
            }

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

            if (string_iswdigit(buffer_str))
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
            //}
        }

        if (CurrentSymbol == '+' || CurrentSymbol == '=')
        {
            next = true;
            switcher = true;
            Nav.push_back(buffer_nav);
        }

        if (CurrentSymbol == '/')
        {
            next = true;
            switcher = false;
        }

        if (CurrentSymbol == ' ')
        {
            Nav.push_back(buffer_nav);
            CurrentSymbol = Object_reader->ReadChar();

            if (CurrentSymbol != L'и' && CurrentSymbol != L'с' && CurrentSymbol != L'т')
            {
                if (buffer_nav)
                {
                    delete buffer_nav;
                    buffer_nav = nullptr;
                }
                throw "NavigationState 2";
            }

            if (CurrentSymbol == L'и')
            {
                buffer_nav->NumberInNextNav = true;
                CurrentSymbol = Object_reader->ReadChar();

                if (CurrentSymbol != ' ')
                {
                    if (buffer_nav)
                    {
                        delete buffer_nav;
                        buffer_nav = nullptr;
                    }
                    throw "NavigationState 3";
                }

                switcher = true;

            } else
            {
                CurrentSymbol = Object_reader->ReadChar();

                if (CurrentSymbol != L'т')
                {
                    if (buffer_nav)
                    {
                        delete buffer_nav;
                        buffer_nav = nullptr;
                    }
                    throw "NavigationState 4";
                }

                CurrentSymbol = Object_reader->ReadChar();

                if (CurrentSymbol != L'р')
                {
                    if (buffer_nav)
                    {
                        delete buffer_nav;
                        buffer_nav = nullptr;
                    }
                    throw "NavigationState 5";
                }

                CurrentSymbol = Object_reader->ReadChar();

                if (CurrentSymbol != '.')
                {
                    if (buffer_nav)
                    {
                        delete buffer_nav;
                        buffer_nav = nullptr;
                    }
                    throw "NavigationState 6";
                }

                CurrentSymbol = Object_reader->ReadChar();

                if (CurrentSymbol != ' ')
                {
                    if (buffer_nav)
                    {
                        delete buffer_nav;
                        buffer_nav = nullptr;
                    }
                    throw "NavigationState 7";
                }

                CurrentSymbol = Object_reader->ReadChar();

                if (CurrentSymbol != L'и')
                {
                    buffer = CurrentSymbol;
                    return;
                }

                CurrentSymbol = Object_reader->ReadChar();

                if (CurrentSymbol != ' ')
                {
                    return;
                }

                buffer_str = "";
                limit = 100;
                while (CurrentSymbol = Object_reader->ReadChar())
                {
                    if (/*iswupper(CurrentSymbol)*/ CurrentSymbol == '.' || CurrentSymbol == '#')
                    {
                        break;
                    }

                    if (CurrentSymbol == '&')
                    {
                        CurrentSymbol = '.';
                    }

                    buffer_str += CurrentSymbol;

                    if (limit == 0)
                    {
                        if (buffer_nav)
                        {
                            delete buffer_nav;
                            buffer_nav = nullptr;
                        }
                        throw "NavigationState 8. Out of limit";
                    }
                    limit--;
                }

                buffer_nav->AddInfo = buffer_str.left(buffer_str.size() - 1);
                Nav.push_back(buffer_nav);

                CurrentSymbol = Object_reader->ReadChar();
                if (CurrentSymbol == '+' || CurrentSymbol == '=')
                {
                    next = true;
                    switcher = true;
                    Nav.push_back(buffer_nav);

                } else
                {
                    buffer = CurrentSymbol;
                    return;
                }
            }

        }

        buffer_str = "";
    }
}

void Parser::PersonState(std::list<Person*>& Persons)
{
    QString buffer_str;
    QString CurrentRole;
    Person* pers = nullptr;
    int limit = 100;

    if (buffer == " ")
    {
        buffer = "";
    }

    if (CheckEmpty(buffer))
    {
        return;
    }

    buffer_str = buffer;
    buffer = "";

    if (buffer_str[0] == '?')
    {
        while (CurrentSymbol = Object_reader->ReadChar())
        {
            if (CurrentSymbol == '&')
            {
                break;
            }

            buffer_str += CurrentSymbol;

            if (limit == 0)
            {
                if (pers)
                {
                    delete pers;
                    pers = nullptr;
                }
                throw "PersonStat 1. Out of limit";
            }
            limit--;
        }

        buffer_str += '.';
        pers = new Person;
        pers->Name = buffer_str;
        Persons.push_back(pers);
        return;
    }

    while (true)
    {
        buffer_str = "";
        limit = 100;
        while (CurrentSymbol = Object_reader->ReadChar())
        {
            if (CurrentSymbol == ':')
            {
                break;
            }

            buffer_str += CurrentSymbol;

            if (limit == 0)
            {
                if (pers)
                {
                    delete pers;
                    pers = nullptr;
                }
                throw "PersonStat 2. Out of limit";
            }
            limit--;
        }

        CurrentRole = buffer_str;
        buffer_str = "";

        CurrentSymbol = Object_reader->ReadChar();

        if (CurrentSymbol != ' ')
        {
            if (pers)
            {
                delete pers;
                pers = nullptr;
            }
            throw "PersonStat 3";
        }

        while (true)
        {
            buffer_str = "";
            limit = 100;
            while (CurrentSymbol = Object_reader->ReadChar())
            {
                if (CurrentSymbol == ',' || CurrentSymbol == '&' || CurrentSymbol == ';')
                {
                    break;
                }

                buffer_str += CurrentSymbol;

                if (limit == 0)
                {
                    if (pers)
                    {
                        delete pers;
                        pers = nullptr;
                    }
                    throw "PersonStat 4. Out of limit";
                }
                limit--;
            }

            pers = new Person;
            pers->Name = buffer_str;
            pers->Role = CurrentRole;
            Persons.push_back(pers);

            if (CurrentSymbol == ';')
            {
                break;
            }

            if (CurrentSymbol == ',')
            {
                CurrentSymbol = Object_reader->ReadChar();

                if (CurrentSymbol != ' ')
                {
                    if (pers)
                    {
                        delete pers;
                        pers = nullptr;
                    }
                    throw "PersonStat 5";
                }
            }

            if (CurrentSymbol == '&')
            {
                return;
            }
        }
    }
}

Content* Parser::ContentState()
{
    Content *object_content = new Content;

    while (CurrentSymbol != '.')
    {
        CategoryState(object_content->Object_category);

        if (CurrentSymbol == '`')
        {
            continue;
        }

        PagesGraphsState(object_content->Object_pages, object_content->Object_graphs, *object_content->Object_category.back());
    }

    FillAllCategories(object_content->Object_category);

    return object_content;
}

void Parser::CategoryState(std::list<Category*>& category)
{
    QString buffer_str;
    int limit = 100;

    while (true)
    {
        buffer_str = "";
        Category* cat = new Category;

        if (CurrentSymbol == ',' && !Object_reader->IsLastCharInString())
        {
            CurrentSymbol = Object_reader->ReadChar();
        }

        limit = 100;
        while (CurrentSymbol = Object_reader->ReadChar())
        {
            if (!iswdigit(CurrentSymbol))
            {
                break;
            }

            buffer_str += CurrentSymbol;

            if (limit == 0)
            {
                if (cat)
                {
                    delete cat;
                    cat = nullptr;
                }
                throw "CategoryState 1. Out of limit";
            }
            limit--;
        }

        cat->Number = buffer_str;
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
            CurrentSymbol = Object_reader->ReadChar();
            if (CurrentSymbol == L'и')
            {
                CurrentSymbol = Object_reader->ReadChar();
                if (CurrentSymbol != ' ')
                {
                    if (cat)
                    {
                        delete cat;
                        cat = nullptr;
                    }
                    throw "CategoryState 2";
                }

                continue;
            }
            buffer = CurrentSymbol;
        }

        return;
    }
}

void Parser::PagesGraphsState(std::list<Pages*>& page, std::list<Graphs*>& graph, Category& category)
{
    QString buffer_str;
    Pages* pg = nullptr;
    int switcher = 0;
    int limit = 100;

    while (true)
    {
        buffer_str = buffer;
        buffer = "";

        limit = 100;
        while (CurrentSymbol = Object_reader->ReadChar())
        {
            if (CurrentSymbol == ',' || CurrentSymbol == '/' || CurrentSymbol == '.' || CurrentSymbol == ';' || CurrentSymbol == ' ' || CurrentSymbol == 8212 || CurrentSymbol == '-')
            {
                break;
            }

            buffer_str += CurrentSymbol;

            if (limit == 0)
            {
                if (pg)
                {
                    if (pg)
                {
                    delete pg;
                    pg = nullptr;
                }
                    pg = nullptr;
                }
                throw "PagesGraphsState 1. Out of limit";
            }
            limit--;
        }

        if (buffer_str.size() != 0) {
            if (switcher == 0)
            {
                pg = new Pages;
                pg->Number_1 = buffer_str;
                category.object_pages.push_back(pg);
                page.push_back(pg);

            } else if (switcher == 2)
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
                pg->Number = buffer_str;
                switcher = 0;

            } else {
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
                switcher = 0;
            }
        }

        if (CurrentSymbol == '.' && !Object_reader->IsLastCharInString())
        {
            CurrentSymbol = Object_reader->ReadChar();
        }

         //1
        if (CurrentSymbol == ',')
        {
            return;
        }

        if (CurrentSymbol == ' ')
        {
            CurrentSymbol = Object_reader->ReadChar();

            if (CurrentSymbol == L'и')
            {
                CurrentSymbol = Object_reader->ReadChar();

                if (CurrentSymbol != ' ')
                {
                    if (pg)
                {
                    delete pg;
                    pg = nullptr;
                }
                    throw "PagesGraphsState 2";
                }
                continue;
            }

            if (CurrentSymbol != L'г')
            {
                buffer = CurrentSymbol;
                continue;
            }

            CurrentSymbol = Object_reader->ReadChar();
            if (CurrentSymbol != L'р')
            {
                if (pg)
                {
                    delete pg;
                    pg = nullptr;
                }
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
            /*Graphs* gr = new Graphs;
            gr->object_category = &category;
            gr->Number_1 = pg->Number_1;
            gr->Number_2 = pg->Number_2;*/

            pg = page.back();
            pg->graphs = true;

            CurrentSymbol = Object_reader->ReadChar();

            if (CurrentSymbol == ',')
            {
                return;
            }

            if (CurrentSymbol == '.')
            {
                if (Object_reader->IsLastCharInString())
                {
                    return;
                }
                CurrentSymbol = Object_reader->ReadChar();

                //1
                if (CurrentSymbol == ';')
                {
                    //graph.push_back(gr);
                    //page.pop_back();
                    continue;
                }

                if (CurrentSymbol == ' ')
                {
                    CurrentSymbol = Object_reader->ReadChar();

                    if (CurrentSymbol == L'и')
                    {
                        CurrentSymbol = Object_reader->ReadChar();

                        if (CurrentSymbol != ' ')
                        {
                            if (pg)
                            {
                                delete pg;
                                pg = nullptr;
                            }
                            throw "PagesGraphsState 5";
                        }
                        //graph.push_back(gr);
                        //page.pop_back();
                        continue;
                    }
                }
            }

            if (CurrentSymbol == ' ')
            {
                CurrentSymbol = Object_reader->ReadChar();

                if (CurrentSymbol != '/')
                {
                    if (pg)
                {
                    delete pg;
                    pg = nullptr;
                }
                    throw "PagesGraphsState 6";
                }
            }

            if (CurrentSymbol == '/')
            {
                buffer_str = "";
                limit = 100;
                while (CurrentSymbol = Object_reader->ReadChar())
                {
                    if (CurrentSymbol == '.' || CurrentSymbol == ',' || CurrentSymbol == ';')
                    {
                        break;
                    }

                    //1подв
                    /*if (CurrentSymbol == ' ')
                    {
                        continue;
                    }*/

                    buffer_str += CurrentSymbol;

                    if (limit == 0)
                    {
                        if (pg)
                        {
                            delete pg;
                            pg = nullptr;
                        }
                        throw "PagesGraphsState 7. Out of limit";
                    }
                    limit--;
                }

                pg->Table_g = buffer_str;
                //graph.push_back(gr);
                //page.pop_back();

                if (CurrentSymbol == ',')
                {
                    return;
                }

                if (CurrentSymbol == ';')
                {
                    switcher = 0;
                    continue;
                }

                 //1
                if (CurrentSymbol == '.')
                {
                    if (Object_reader->IsLastCharInString())
                    {
                        return;
                    }
                    CurrentSymbol = Object_reader->ReadChar();

                    if (CurrentSymbol == ';')
                    {
                        continue;
                    }

                    if (CurrentSymbol == ',')
                    {
                        return;
                    }

                    if (CurrentSymbol == ' ')
                    {
                        CurrentSymbol = Object_reader->ReadChar();

                        if (CurrentSymbol == L'и')
                        {
                            CurrentSymbol = Object_reader->ReadChar();

                            if (CurrentSymbol != ' ')
                            {
                                if (pg)
                                {
                                    delete pg;
                                    pg = nullptr;
                                }
                                throw "PagesGraphsState 8";
                            }
                            continue;
                        }
                    }
                }
            }
        }

        if (CurrentSymbol == '/')
        {
            switcher = 2;
        }

        if (CurrentSymbol == ';')
        {
            continue;
        }

        if (CurrentSymbol == '.')
        {
            return;
        }

        if (CurrentSymbol == 8212 || CurrentSymbol == '-')
        {
            switcher = 1;
        }
    }

}

void Parser::FillAllCategories(std::list<Category*>& categories)
{
    std::list<Pages*> pages;
    std::list<Category*> cat;
    bool flag = false;

    for (Category* category : categories)
    {
        if (category->object_pages.size() == 0)
        {
            flag = true;
            cat.push_back(category);
        }

        pages.clear();

        for (Pages* page : category->object_pages)
        {
            pages.push_back(page);
        }

        if (category->object_pages.size() != 0 && flag)
        {
            for (Category* c : cat)
            {
                c->object_pages = pages;
            }

            cat.clear();
            flag = false;
        }
    }
}
