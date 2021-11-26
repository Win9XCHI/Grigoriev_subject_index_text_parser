#include "unparser.h"

UnParser::UnParser()
{

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

                for (QString object_person : object_persons)
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
    QString str = "INSERT provinces(Name)\0            VALUES\0            ('" + CurrentProvince + "');";
    Object_writer.OutputString(str);
}

void UnParser::OAssemblies()
{
    QString str = "SELECT @id_provinces := MAX(id) FROM provinces WHERE Name = 'Екатеринославская губ.';\0            INSERT assemblies(Name, Provinces_id)\0            VALUES\0";

    for (auto& item : Assemblies)
    {
        str += "('" + item.Name + "', @id_provinces),\0";
    }

    str[str.length() - 2] = ';';

    Object_writer.OutputString(str);
}

void UnParser::OPersons(std::list<Person*> persons)
{
    QString str = "INSERT persons(Name) \0            VALUES\0";

    for (auto& item : persons)
    {
        str += "('" + item->Name + "'),\0";
    }

    str[str.length() - 2] = ';';

    Object_writer.OutputString(str);
}

void UnParser::OWork(Work* work)
{
    QString assebly;

    for (auto& ass : Assemblies)
    {
        for (auto& num : ass.numbers)
        {
            if (num == work->NumberInBook)
            {
                assebly = ass.Name;
            }
        }

    }

    QString str = "SELECT @id_assemblies := MAX(id) FROM assemblies WHERE Name = '" + assebly + "' AND Provinces_id = @id_provinces;\0";
    str += "INSERT works(Name, Year_editions, Year_creation, City, Assemblies_id)\0            VALUES\0";

    str += "('" + work->name + "', '" + work->year + "', NULL, '" + work->city + "', @id_assemblies);";

    Object_writer.OutputString(str);
}

void UnParser::OPointer(Pointer* point)
{
    QString lastWork = "SELECT @id_last_work := MAX(id) FROM works;\0\0";

    QString str = "INSERT pointer(Part, Number, Page, Works_id)\0            VALUES\0";
    str += "(" + QString::number(point->Part) + ", " + QString::number(point->Number) + ", " + QString::number(point->Page) + ", @id_last_work);";
    Object_writer.OutputString(lastWork + str);
}

void UnParser::ONavigation(std::list<Navigation*> nav)
{
    QString str = "INSERT navigation(Number, Rome, Arabic, Works_id) \0            VALUES\0";

    for (auto& item : nav)
    {
        str += "('" + item->Number + "', '" + item->Rome + "', " + QString::number(item->Arabic) + ", @id_last_work),\0";
    }
    str[str.length() - 2] = ';';

    Object_writer.OutputString(str);
}

void UnParser::OUnits(std::list<Account_unit*> unit)
{
    QString str = "INSERT account_units(Type, Number, Name, Works_id, Account_units_id)\0            VALUES\0";

    for (auto& item : unit)
    {
        str += "('" + item->Type + "', " + item->Number + ", '" + item->Name + "', @id_last_work, NULL),\0";
    }

    str[str.length() - 2] = ';';

    Object_writer.OutputString(str);
}

void UnParser::OCounties()
{

}

void UnParser::OC_W()
{

}

void UnParser::OP_W(Person* pers)
{
    QString str = "SELECT @id_person := MAX(id) FROM persons WHERE Name = '" + pers->Name + "';\0";
    str += "INSERT p_w(Role, Persons_id, Works_id)\0            VALUES\0";
    str += "('" + pers->Role + "', @id_person, @id_last_work);";

    Object_writer.OutputString(str);
}

void UnParser::OContent_Pages_Graphs(std::list<Content*> contents)
{
    for (auto& content : contents)
    {
        for (auto& cat : content->Object_category)
        {
            QString str = "INSERT content(Works_id, Categories_id)\0            VALUES (@id_last_work, Search_categories_f('" + cat->Number + "'));\0";
            QString pages = "SELECT @id_content := MAX(id) FROM content;\0                    INSERT pages(Number_1, Number_2, Number, Content_id)\0";
            QString graphs = "SELECT @id_content := MAX(id) FROM content;\0                    INSERT graphs(Number_1, Number_2, Table_g, Content_id)\0";

            for (auto& item : cat->object_pages)
            {
                if (!item->graphs)
                {
                    pages += "VALUES ('" + item->Number_1 + "', " + item->Number_2 + ", " + item->Number + ", @id_content),\0";
                    continue;
                }

                graphs += "VALUES ('" + item->Number_1 + "', " + item->Number_2 + ", " + item->Table_g + ", @id_content),\0";
            }

            pages[pages.length() - 2] = ';';
            graphs[graphs.length() - 2] = ';';

            Object_writer.OutputString(str);
            Object_writer.OutputString(pages);
            Object_writer.OutputString(graphs);
        }
    }
}

