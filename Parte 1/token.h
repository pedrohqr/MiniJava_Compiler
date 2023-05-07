#ifndef _TOKEN_H
#define _TOKEN_H

#include <iostream>
#include "String.h"

#pragma pack(1) // Alinhamento

enum class TokenType
{
    UNDEF = -1,
    ID,
    INTEGER_LITERAL,
    OP,
    SEP,
    KEYWORD,
    STRING,
    END_OF_FILE
};

enum class SeparatorIndex
{
    LPAREN,     // '('
    RPAREN,     // ')'
    LBRACKET,   // '['
    RBRACKET,   // ']'
    LBRACE,     // '{'
    RBRACE,     // '}'
    SEMICOLON,  // ';'
    COMMA,      // ','
    PERIOD      // '.'
};

const char Separators[] = {'(', ')', '[', ']', '{', '}', ';', ',', '.'};
// Length of Separators array
constexpr size_t SEPArrayLen = 9;

enum class KeyWordIndex
{
    BOOLEAN,
    CLASS,
    ELSE,
    EXTENDS,
    FALSE,
    IF,
    INT,
    LENGTH,
    MAIN,
    NEW,
    PUBLIC,
    RETURN,
    STATIC,
    STRING,
    SYSOUT, // System.out.println
    THIS,
    TRUE,
    VOID,
    WHILE
};

const my::String KeyWords[] = {"boolean", "class", "else", "extends", "false", "if", "int", "length", "main", "new", "public", 
                               "return", "static", "String", "System.out.println", "this", "true", "void", "while"};
// Length of KeyWord array
constexpr size_t KWArrayLen = 19;

enum class OperatorIndex
{
    AND,    // &
    LESS,   // <
    GRATER, // >
    PLUS,   // +
    MINUS,  // -
    STAR,   // *
    BAR,    // /
    ATRIB,  // =    
    NOT     // !
};

const char Operators[] = {'&', '<', '>', '+', '-', '*', '/', '=', '!'};
// Length of Operators array
constexpr size_t OPArrayLen = 9;

/* Standart Token base class */
class Token
{
public:
    TokenType type;

    Token(TokenType t): type{t}
    {

    }

    virtual ~Token()
    {
        
    }
};

/* KeyWord Token */
class TKeyWord : public Token
{
public:
    int index;

    TKeyWord(TokenType type, const int index): Token{type}, index{index}
    {

    }

    ~TKeyWord(){}
};

/* ID Token */
class TID : public Token
{
public:
    my::String value;

    TID(TokenType type, const my::String& val): Token{type}, value{val}
    {

    }
    ~TID()
    {

    }
};

/* String Token */
class TString : public Token
{
public:
    std::string value;

    TString(TokenType type, const std::string& str): Token{type}, value{str}
    {

    }

    ~TString()
    {

    }
};

/* Number token of type T */
template<typename T>
class TNumber : public Token
{
public:
    T value;

    TNumber(TokenType type, const T& val): Token{type}, value{val}
    {

    }

    ~TNumber()
    {

    }
};

/* Separator Token */
class TSeparator : public Token
{
public:
    unsigned char c;

    TSeparator(TokenType type, const unsigned char& i): Token{type}, c{i}
    {

    }

    ~TSeparator()
    {

    }
};

/* Operator Token */
class TOperator : public Token
{
public:
    my::String value;

    TOperator(TokenType type, const my::String& val): Token{type}, value{val}
    {

    }

    ~TOperator()
    {

    }
};

/* Undefined Token */
class TUndefToken: public Token
{
public:
    char c;

    TUndefToken(TokenType type, const char& c): Token{type}, c{c}
    {

    }

    ~TUndefToken()
    {

    }
};

#endif