#include "parser.h"

Parser::Parser()
{

}

bool Parser::MainState()
{
    try {
        FirstPageNumber();

        ProvinceState();

        while (!Object_reader.IsEOF())
        {
            WorkState();
        }
    } catch (bool flag) {
        return false;

    } catch (QString str) {
        return false;
    }

    return true;
}

void Parser::FirstPageNumber()
{
    QString number;
    char symbol = Object_reader.ReadChar();

    if (symbol != '-')
    {
        throw Object_reader.Error();
    }

    while (symbol = Object_reader.ReadChar() && isdigit(symbol))
    {
        number += symbol;
    }

    if (symbol != '-')
    {
        throw Object_reader.Error();
    }

    while (symbol = Object_reader.ReadChar() && symbol != '\0') {}
}

void Parser::ProvinceState()
{

}

void Parser::WorkState()
{

}



void Parser::PoinerState()
{

}

void Parser::PersonState()
{

}

void Parser::PagesState()
{

}

void Parser::NavigationState()
{

}

void Parser::GraphsState()
{

}

void Parser::CountyState()
{

}

void Parser::Account_unitState()
{

}

void Parser::ContentState()
{

}
