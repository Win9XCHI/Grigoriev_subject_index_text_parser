#ifndef READER_H
#define READER_H
#include <iostream>
#include <fstream>
#include <qstring.h>
#include <locale>
#include <codecvt>
#include <sstream>
#include <stdio.h>

class Reader
{
public:
    bool bOpen;
    std::string filename;
    std::wifstream file;
    std::ofstream ErrorFile;
    int NumberOfChar;
    int CountOfString;
    int NumberOfCharInString;
    FILE* pFile;

    Reader();

    bool OpenFile();
    wchar_t ReadChar(bool bIgnoreEnter = true);
    void CloseFile();
    void Error(QString str = "");
    bool IsEOF();
};

#endif // READER_H
