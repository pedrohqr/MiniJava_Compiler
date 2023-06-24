#include "stentry.h"

STEntry::STEntry()
{
    // do nothing
}
        
STEntry::STEntry(Token* tok) :
    token{tok},
    reserved{false}
{
    // do nothing
}

STEntry::STEntry(Token* tok, bool res) :
    token{tok},
    reserved{res}
{
    // do nothing
}

STEntry::~STEntry()
{
    if (token != nullptr)
        delete token;
    
}