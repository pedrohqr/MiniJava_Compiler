#include <iostream>
#include <ctype.h> // Funções de caracteres
#include <errno.h> // Tratamento de erros
//#include <string.h>
#include <cstring>

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
    STRING,
    END_OF_FILE
};

enum class Separator : char
{
    LPAREN = '(',
    RPAREN = ')',
    LBRACKET = '[',
    RBRACKET = ']',
    LBRACE = '{',
    RBRACE = '}',
    SEMICOLON = ';',
    COMMA = ',',
    PERIOD = '.'
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

        // Identificador de strings
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

        Token(int name, char c)
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = c;
        }
};
