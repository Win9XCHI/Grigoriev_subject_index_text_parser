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

wchar_t Reader::ReadChar(bool bIgnoreEnter)
{
    if (!bOpen)
    {
        throw "File is close";
    }

    wchar_t symbol = ' ';

    while (true)
    {
        if (file.eof())
        {
            throw "End of file";
        }

        file.get(symbol);
        NumberOfChar++;

        if (symbol != '\0')
        {
            break;
        }

        if (symbol == '\0')
        {
            CountOfString++;
            NumberOfCharInString = 1;

            if (!bIgnoreEnter)
            {
                return symbol;
            }
        }
    }

    return symbol;
}

void Reader::Error(QString str)
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
