#include "unparser.h"

UnParser::UnParser()
{

}

bool UnParser::MainState()
{
    Object_writer.OutputFilename = "Yekaterinoslav.sql";

    try {


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

}

void UnParser::OProvinces()
{

}

void UnParser::OAssemblies()
{

}

void UnParser::OPersons()
{

}

void UnParser::OWork()
{

}

void UnParser::OPointer()
{

}

void UnParser::ONavigation()
{

}

void UnParser::OUnits()
{

}

void UnParser::OCounties()
{

}

void UnParser::OC_W()
{

}

void UnParser::OP_W()
{

}

void UnParser::OContent_Pages_Graphs()
{

}

