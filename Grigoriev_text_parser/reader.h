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
    bool Error(QString str = "");
    bool IsEOF();
};

#endif // READER_H
