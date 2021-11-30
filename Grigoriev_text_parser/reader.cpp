#include "reader.h"

Reader::Reader()
{
    bOpen = OpenFile();
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
    bool comment = false;

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

        if (StringFile.size() <= CountOfString)
        {
            throw "The end";
        }

        c = StringFile[CountOfString][NumberOfCharInString];

        NumberOfChar++;
        NumberOfCharInString++;

        if (c == '|' && !comment)
        {
            comment = true;
            continue;
        }

        if (c == '|' && comment)
        {
            comment = false;
            continue;
        }

        if (comment)
        {
            continue;
        }

        if (NumberOfCharInString != 0)
        {
            break;
        }
    }

    return c;
}

void Reader::Error(QString str)
{

}

bool Reader::IsEOF()
{
    return StringFile.size() == CountOfString;
}
