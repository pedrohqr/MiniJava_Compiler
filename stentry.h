#include "token.h"
#include <memory>

class STEntry 
{
public:
    Token* token;
    bool reserved;

    STEntry();
    STEntry(Token*);
    STEntry(Token*, bool);
    ~STEntry();
};
