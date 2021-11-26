#ifndef UNPARSER_H
#define UNPARSER_H
#include <parser.h>

class UnParser : public Parser
{
public:
    UnParser();

    bool MainState() override;
    void OUseDB();
    void OProvinces();
    void OAssemblies();
    void OPersons();
    void OWork();
    void OPointer();
    void ONavigation();
    void OUnits();
    void OCounties();
    void OC_W();
    void OP_W();
    void OContent_Pages_Graphs();
};

#endif // UNPARSER_H
