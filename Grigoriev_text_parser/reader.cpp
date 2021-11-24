#include "reader.h"

Reader::Reader() : filename("ForParse.txt"), NumberOfChar(0), CountOfString(0), NumberOfCharInString(0)
{
    bOpen = OpenFile();
    ErrorFile.open("Error_log.txt");
    ErrorFile.clear();
    ErrorFile.close();
}

bool Reader::OpenFile()
{
    if (filename.empty()) {
        return false;
    }

    file.open(filename);

    if (!file.is_open()) {
        return false;
    }

    std::wstring inputString;
    while(!file.eof())
    {
        getline(file, inputString);
        StringFile.push_back(inputString);
    }

    file.close();

    return true;
}

void Reader::CloseFile()
{
    file.close();
}

bool Reader::IsLastCharInString()
{
    return NumberOfCharInString == StringFile[CountOfString].length();
}

wchar_t Reader::ReadChar(bool bIgnoreEnter)
{
    if (StringFile.size() <= CountOfString)
    {
        throw "The end";
    }

    wchar_t c;

    while (true)
    {
        if (StringFile[CountOfString].length() == NumberOfCharInString)
        {
            CountOfString++;
            NumberOfCharInString = 0;

            if (!bIgnoreEnter)
            {
                return '\0';
            }
        }

        c = StringFile[CountOfString][NumberOfCharInString];

        NumberOfChar++;
        NumberOfCharInString++;

        if (NumberOfCharInString != 0)
        {
            break;
        }
    }

    return c;
}

void Reader::Error(QString str)
{
    if (!ErrorFile.is_open())
    {
        ErrorFile.open("Error_log.txt");
    }

    ErrorFile << CountOfString + 1 << ":" << NumberOfCharInString + 1 << " = " << str.toStdString();

    ErrorFile.close();
}

bool Reader::IsEOF()
{
    return StringFile.size() == CountOfString;
}
