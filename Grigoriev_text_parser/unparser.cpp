#include "unparser.h"

UnParser::UnParser() : counter(0)
{

}

QString UnParser::InfoOrNull(QString str)
{
    return str == "" ? "NULL" : "'" + str + "'";
}

int UnParser::InfoOrNull(int number)
{
    return number;
}

bool UnParser::MainState()
{
    Object_writer.OutputFilename = "Yekaterinoslav.sql";
    Object_writer.OpenFile();

    try {
        OUseDB();
        OProvinces();
        OAssemblies();

        std::list<QString> object_persons;
        std::list<Person*> persons;
        for (auto& Work : Works)
        {
            for (auto& Person : Work->Object_person)
            {
                bool flag = true;

                for (QString& object_person : object_persons)
                {
                    if (object_person == Person->Name) {
                        flag = false;
                    }
                }

                if (flag)
                {
                    object_persons.push_back(Person->Name);
                    persons.push_back(Person);
                }
            }

            OPersons(persons);
            OWork(Work);
            OPointer(Work->Object_pointer);
            ONavigation(Work->Object_navigation);
            OUnits(Work->Object_account_unit);

            for (auto& Person : Work->Object_person)
            {
                OP_W(Person);
            }

            OContent_Pages_Graphs(Work->Object_content);
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

    Object_writer.CloseFile();

    return true;
}

void UnParser::OUseDB()
{
    Object_writer.OutputString("USE db_statistics;");
}

void UnParser::OProvinces()
{
    Object_writer.OutputString("INSERT provinces(Name) VALUES (" + InfoOrNull(CurrentProvince) + ");");
}

void UnParser::OAssemblies()
{
    if (Assemblies.size() > 0)
    {
        QString str;
        Object_writer.OutputString("SELECT @id_provinces := MAX(id) FROM provinces WHERE Name = " + InfoOrNull(CurrentProvince) + ";");

        Object_writer.OutputString("INSERT assemblies(Name, Provinces_id) VALUES");

        counter = 0;
        for (auto& item : Assemblies)
        {
            counter++;
            str = "(" + InfoOrNull(item.Name) + ", @id_provinces)";

            if (counter == Assemblies.size())
            {
                str += ';';
            } else
            {
                str += ',';
            }

            Object_writer.OutputString(str);
        }
    }
}

void UnParser::OPersons(std::list<Person*> persons)
{
    QString str;
    Object_writer.OutputString("INSERT persons(Name) VALUES");

    counter = 0;
    for (auto& item : persons)
    {
        counter++;
        str = "(" + InfoOrNull(item->Name) + ")";

        if (counter == persons.size())
        {
            str += ';';
        } else
        {
            str += ',';
        }

        Object_writer.OutputString(str);
    }   
}

void UnParser::OWork(Work* work)
{
    QString assebly = "";

    for (auto& ass : Assemblies)
    {
        for (auto& num : ass.numbers)
        {
            if (num == work->NumberInBook)
            {
                assebly = ass.Name;
                break;
            }
        }

        if (assebly != "")
        {
            break;
        }
    }

    Object_writer.OutputString("SELECT @id_assemblies := MAX(id) FROM assemblies WHERE Name = '" + assebly + "' AND Provinces_id = @id_provinces;");

    Object_writer.OutputString("INSERT works(Name, Year_editions, Year_creation, City, Assemblies_id) VALUES");

    Object_writer.OutputString("(" + InfoOrNull(work->name) + ", " + InfoOrNull(work->year) + ", NULL, " + InfoOrNull(work->city) + ", @id_assemblies);");
}

void UnParser::OPointer(Pointer* point)
{
    Object_writer.OutputString("SELECT @id_last_work := MAX(id) FROM works;");
    Object_writer.OutputString("INSERT pointer(Part, Number, Page, Works_id) VALUES");

    QString str = "(" + InfoOrNull(QString::number(point->Part)) + ", " + InfoOrNull(QString::number(point->Number)) + ", " + InfoOrNull(QString::number(point->Page)) + ", @id_last_work);";
    Object_writer.OutputString(str);
}

void UnParser::ONavigation(std::list<Navigation*> nav)
{
    if (nav.size() > 0)
    {
        QString str;
        Object_writer.OutputString("INSERT navigation(Number, Rome, Arabic, Works_id) VALUES");

        counter = 0;
        for (auto& item : nav)
        {
            counter++;
            str = "(" + InfoOrNull(item->Number) + ", " + InfoOrNull(item->Rome) + ", " + InfoOrNull(QString::number(item->Arabic)) + ", @id_last_work)";

            if (counter == nav.size())
            {
                str += ';';
            } else
            {
                str += ',';
            }

            Object_writer.OutputString(str);
        }
    }
}

void UnParser::OUnits(std::list<Account_unit*> unit)
{
    if (unit.size() > 0)
    {
        QString str;
        Object_writer.OutputString("INSERT account_units(Type, Number, Name, Works_id, Account_units_id) VALUES");

        counter = 0;
        for (auto& item : unit)
        {
            counter++;
            str = "(" + InfoOrNull(item->Type) + ", " + InfoOrNull(item->Number) + ", " + InfoOrNull(item->Name) + ", @id_last_work, NULL)";

            if (counter == unit.size())
            {
                str += ';';
            } else
            {
                str += ',';
            }

            Object_writer.OutputString(str);
        }
    }
}

void UnParser::OCounties()
{

}

void UnParser::OC_W()
{

}

void UnParser::OP_W(Person* pers)
{
    Object_writer.OutputString("SELECT @id_person := MAX(id) FROM persons WHERE Name = " + InfoOrNull(pers->Name) + ";");
    Object_writer.OutputString("INSERT p_w(Role, Persons_id, Works_id) VALUES");
    Object_writer.OutputString("(" + InfoOrNull(pers->Role) + ", @id_person, @id_last_work);");
}

void UnParser::OContent_Pages_Graphs(std::list<Content*> contents)
{
    for (auto& content : contents)
    {
        for (auto& cat : content->Object_category)
        {
            QString str;
            unsigned long long countPage = cat->CountOfPage();
            unsigned long long countGraph = cat->CountOfGraphs();
            Object_writer.OutputString("INSERT content(Works_id, Categories_id) VALUES (@id_last_work, Search_categories_f(" + InfoOrNull(cat->Number) + "));");

            if (countPage > 0)
            {
                Object_writer.OutputString("SELECT @id_content := MAX(id) FROM content;");
                Object_writer.OutputString("INSERT pages(Number_1, Number_2, Number, Content_id) VALUES");

                counter = 0;
                for (auto& item : cat->object_pages)
                {
                    if (!item->graphs)
                    {
                        counter++;
                        str = "(" + InfoOrNull(item->Number_1) + ", " + InfoOrNull(item->Number_2) + ", " + InfoOrNull(item->Number) + ", @id_content)";

                        if (counter == countPage)
                        {
                            str += ';';
                        } else
                        {
                            str += ',';
                        }

                        Object_writer.OutputString(str);
                    }
                }
            }

            if (countGraph > 0)
            {
                Object_writer.OutputString("SELECT @id_content := MAX(id) FROM content;");
                Object_writer.OutputString("INSERT graphs(Number_1, Number_2, Table_g, Content_id) VALUES");

                counter = 0;
                for (auto& item : cat->object_pages)
                {
                    if (item->graphs)
                    {
                        counter++;
                        str = "(" + InfoOrNull(item->Number_1) + ", " + InfoOrNull(item->Number_2) + ", " + InfoOrNull(item->Table_g) + ", @id_content)";

                        if (counter == countGraph)
                        {
                            str += ';';
                        } else
                        {
                            str += ',';
                        }

                        Object_writer.OutputString(str);
                    }
                }
            }
        }
    }
}

