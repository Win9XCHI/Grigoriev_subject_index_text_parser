#ifndef READER_H
#define READER_H
#include <iostream>
#include <fstream>
#include <qstring.h>

class Reader
{
public:
    bool bOpen;
    std::string filename;
    std::ifstream file;
    std::ofstream ErrorFile;
    int NumberOfChar;
    int CountOfString;
    int NumberOfCharInString;

    Reader();

    bool OpenFile();
    char ReadChar();
    void CloseFile();
    void Error(QString str);
};

#endif // READER_H