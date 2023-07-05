#include "symboltable.h"
#include <cassert>

//Construtor que instancia uma nova tabela sem especificar uma tabela pai.
SymbolTable::SymbolTable() :
    _parent{nullptr}
{
    // do nothing
}

//Construtor que instancia uma nova tabela especificando uma tabela pai.
SymbolTable::SymbolTable(SymbolTable* p) :
    _parent{p}
{
    // do nothing
}

SymbolTable::~SymbolTable()
{
    _symbols.clear();
}

//Tenta adicionar um novo símbolo à tabela corrente. Primeiramente, verifica
//se já existe uma entrada para o lexema na tabela corrente e, caso haja,
//a inserção não é realizada e o método retorna false; caso contrário, a 
//inserção é efetivada e true é retornado.
bool 
SymbolTable::add(STEntry* t)
{
    assert( t->token->type == TokenType::ID );
    std::string* ptr = (std::string*)t->token->value;

    if ( !( _symbols.find( *ptr ) == _symbols.end() ) )
        return false;
    
    _symbols.insert( {*ptr, t} );
    return true;
}

//Tenta remover um símbolo da tabela.
inline
bool 
SymbolTable::remove(std::string id)
{
    return _symbols.erase(id) != 0;
}

//Limpa a tabela.
inline
void 
SymbolTable::clear()
{
    _symbols.clear();
}

//Verifica se a tabela está vazia.
inline
bool 
SymbolTable::isEmpty() const
{
    return _symbols.empty();
}

//Busca uma entrada equivalente ao lexema passado como parâmetro. A busca se
//inicia no escopo corrente e sobe na hierarquia enquanto não encontrar o
//símbolo, possivelmente até o escopo global. Retorna um ponteiro para a 
//entrada encontrada ou 0, caso o símbolo não esteja presente.
STEntry* 
SymbolTable::find(std::string name)
{
    SymbolTable* table = this;

    auto s = table->_symbols.find(name);

    while (s == table->_symbols.end())
    {
        table = table->_parent;
        if (table == 0)
            return 0;

        s = table->_symbols.find(name);
    } 
    
    return s->second;
}

SymbolTable*
SymbolTable::getParent()
{
    return _parent;
}
