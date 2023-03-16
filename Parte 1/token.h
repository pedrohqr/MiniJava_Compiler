#include <iostream>
#include <ctype.h> // Funções de caracteres
#include <errno.h> // Tratamento de erros
#include <string.h>

using namespace std;

//Nomes e atributos dos tokens da linguagem
enum Names 
{
    UNDEF,
    ID,
    INTEGER_LITERAL,
    OP,
    SEP,
    KEYWORD,
    END_OF_FILE
};

class Token
{
    public: 
        int name;
        int attribute;
        string lexeme;
    
        Token(int name)
        {
            this->name = name;
            attribute = UNDEF;
        }

        Token(int name, string l)
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
        }
        
        Token(int name, int attr)
        {
            this->name = name;
            attribute = attr;
        }
};
