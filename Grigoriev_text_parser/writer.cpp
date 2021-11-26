#include "writer.h"

Writer::Writer()
{
    ErrorFile.open("Error_log.txt");
    ErrorFile.clear();
    ErrorFile.close();
}

void Writer::Error(QString str)
{
    if (!ErrorFile.is_open())
    {
        ErrorFile.open("Error_log.txt");
    }

    ErrorFile << CountOfString + 1 << ":" << NumberOfCharInString + 1 << " = " << str.toStdString();

    ErrorFile.close();
}

void Writer::CloseFile()
{
    if (ErrorFile.is_open())
    {
        ErrorFile.close();
    }

    OutputFile.close();
}

bool Writer::OpenFile()
{
    if (OutputFilename.empty()) {
        return false;
    }

    OutputFile.open(OutputFilename);

    if (!OutputFile.is_open()) {
        return false;
    }

    return true;
}

void Writer::OutputString(QString str)
{
    OutputFile << str.toStdWString() << '\0';
}

void Writer::OutputChar(const wchar_t c)
{
    OutputFile << c;
}

void Writer::OutputBlock(const wchar_t* str)
{
    OutputFile.write(str, sizeof(wcslen(str)));
}
