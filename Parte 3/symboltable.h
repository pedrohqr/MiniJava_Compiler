#include <map>
#include "stentry.h"

class SymbolTable
{
private:
    SymbolTable* _parent;
    std::map<std::string, STEntry*> _symbols;

public:
    SymbolTable();
    SymbolTable(SymbolTable*);
    ~SymbolTable();

    bool add(STEntry*);
    inline bool remove(std::string);
    inline void clear();
    inline bool isEmpty() const;
    STEntry* find(std::string);
    SymbolTable* getParent();
};
