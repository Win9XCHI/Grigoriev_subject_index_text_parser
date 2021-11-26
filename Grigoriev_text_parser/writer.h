#ifndef WRITER_H
#define WRITER_H
#include <reader.h>

class Writer : public Reader
{
    std::ofstream ErrorFile;
    std::wofstream OutputFile;
public:
    std::string OutputFilename;

    Writer();

    bool OpenFile() override;
    void CloseFile() override;
    void Error(QString str = "") override;
    void OutputString(QString str);
    void OutputBlock(const wchar_t* str);
    void OutputChar(const wchar_t c);
};

#endif // WRITER_H
