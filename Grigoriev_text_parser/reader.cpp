#include "reader.h"

Reader::Reader() : bOpen(OpenFile()), filename("ForParse.txt"), NumberOfChar(1), CountOfString(1), NumberOfCharInString(1)
{
}

bool Reader::OpenFile()
{
    if (!filename.empty()) {
        return false;
    }

    file.open(filename);

    if (!file.is_open()) {
        return false;
    }

    return true;
}

void Reader::CloseFile()
{
    file.close();
}

char Reader::ReadChar()
{
    if (!bOpen)
    {
        throw "File is close";
    }

    char symbol = ' ';

    if (file.eof())
    {
        throw "End of file";
    }

    file.get(symbol);
    NumberOfChar++;

    if (symbol == '\0')
    {
        CountOfString++;
        NumberOfCharInString = 1;
    }

    return symbol;
}

bool Reader::Error(QString str)
{
    if (!ErrorFile.is_open())
    {
        ErrorFile.open("Error_log.txt");
    }

    ErrorFile << CountOfString << ":" << NumberOfCharInString << " = " << str.toStdString();
}

bool Reader::IsEOF()
{
    return file.eof();
}
