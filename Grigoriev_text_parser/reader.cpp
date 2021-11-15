#include "reader.h"

Reader::Reader() : filename("ForParse.txt"), NumberOfChar(1), CountOfString(1), NumberOfCharInString(1)
{
    //bOpen = OpenFile();
    bOpen = true;
    pFile = fopen ("ForParse.txt" , "r");
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

    return true;
}

void Reader::CloseFile()
{
    file.close();
}

wchar_t Reader::ReadChar(bool bIgnoreEnter)
{

    std::vector<std::wstring> inputVector;
        std::wstring inputString, result;
        std::wifstream inputStream;
        inputStream.open("ForParse.txt");
        while(!inputStream.eof())
        {
            getline(inputStream, inputString);
            inputVector.push_back(inputString);
        }
        inputStream.close();

        srand(time(NULL));
        int numLines = rand() % inputVector.size();
        for(int i = 0; i < numLines; i++)
        {
            int randomLine = rand() % inputVector.size();
            result += inputVector[randomLine];
        }

        std::wofstream resultStream;
        resultStream.open("result.txt");
        resultStream << result;
        resultStream.close();

    /*if (!bOpen)
    {
        throw "File is close";
    }

    //char symbol_c[2];
    wchar_t symbol[100];

    while (true)
    {
        if (file.eof())
        {
            throw "End of file";
        }

        file.imbue(std::locale( ".1251" ));
        std::wstringstream wss;
        wss << file.rdbuf();
        std::wstring st = wss.str();
        std::wcout << st;

        //file.get(symbol);

        if (fgetws (symbol, 1, pFile) != NULL)
        {
            fputws ( symbol, stdout );
        }

        //symbol_c[1] = '\0';

        //symbol = (wchar_t)*symbol_c;
        //mbstowcs(symbol, symbol_c, 1);
        NumberOfChar++;
        NumberOfCharInString++;

        if (*symbol != '\0')
        {
            break;
        }

        if (*symbol == '\0')
        {
            CountOfString++;
            NumberOfCharInString = 1;

            if (!bIgnoreEnter)
            {
                return *symbol;
            }
        }
    }

    return *symbol;*/
}

void Reader::Error(QString str)
{
    if (!ErrorFile.is_open())
    {
        ErrorFile.open("Error_log.txt");
    }

    ErrorFile << CountOfString << ":" << NumberOfCharInString << " = " << str.toStdString();

    ErrorFile.close();
}

bool Reader::IsEOF()
{
    return file.eof();
}
