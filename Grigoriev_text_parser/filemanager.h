#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <iostream>
#include <qstring.h>

class FileManager
{
public:
    bool bOpen;
    std::string filename;
    int NumberOfChar;
    int CountOfString;
    int NumberOfCharInString;

    FileManager();

    virtual bool OpenFile() = 0;
    virtual wchar_t ReadChar(bool bIgnoreEnter = true) = 0;
    virtual void CloseFile() = 0;
    virtual void Error(QString str = "") = 0;
    virtual bool IsEOF() = 0;
};

#endif // FILEMANAGER_H
