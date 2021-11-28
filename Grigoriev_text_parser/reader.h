#ifndef READER_H
#define READER_H
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include "filemanager.h"

class Reader : public FileManager
{
public:
    std::wifstream file;
    std::vector<std::wstring> StringFile;

    Reader();

    bool OpenFile() override;
    wchar_t ReadChar(bool bIgnoreEnter = true) override;
    void CloseFile() override;
    void Error(QString str = "") override;
    bool IsEOF() override;

    bool IsLastCharInString();
};

#endif // READER_H
