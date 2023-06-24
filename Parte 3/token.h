#ifndef _TOKEN_H
#define _TOKEN_H

#include <iostream>
#include <array>

#pragma pack(1) // Alignment

constexpr auto maxLen = 1024;

enum class TokenType
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

constexpr std::array<const char*, 8> TokenTypeNames{"UNDEFINED", "IDENTIFIER", "INTEGER LITERAL", "OPERATOR", "SEPARATOR", "KEYWORD", "STRING", "END OF FILE"};

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

constexpr std::array<const char, 9> Separators{'(', ')', '[', ']', '{', '}', ';', ',', '.'};

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

constexpr std::array<const char*, 19> KeyWords = {"boolean", "class", "else", "extends", "false", "if", "int", "length", "main", "new", "public", 
                                                          "return", "static", "String", "System.out.println", "this", "true", "void", "while"};

enum class OperatorIndex
{
    AND,    // &&
    LESS,   // <
    GRATER, // >
    PLUS,   // +
    MINUS,  // -
    STAR,   // *
    BAR,    // /
    ATRIB,  // = 
    EQUALS, // ==   
    NEQUAL, // !=
    NOT     // !
};

constexpr std::array<const char*, 11> Operators = {"&&", "<", ">", "+", "-", "*", "/", "=", "==", "!=", "!"};

class Token
{
public:
    TokenType type;

    union
    {
        int integer;
        void* value;
        OperatorIndex opIndex;
        SeparatorIndex sepIndex;
        KeyWordIndex kwIndex;
    };
    

    Token(TokenType t):
        type{t},
        value{this}
    {
        // do nothing
    }

    /*
        Constructor of IDs or KEYWORDs.
    */
    Token(const std::string str)
    {
        // Find the keyword index
        for (unsigned short i = 0; i < KeyWords.size(); i++)
            if (str == KeyWords[i])
            {
                kwIndex = (KeyWordIndex)i;
                type = TokenType::KEYWORD;
                return;
            }

        // If not found keyword index, then it's an ID
        type = TokenType::ID;
        value = new std::string{str};
    }

    /*
        Constructor of STRINGs, OPERATORs or INTEGER_LITERALs.
    */
    Token(TokenType tokenType, const std::string str):
        type{tokenType}
    {
        switch (tokenType)
        {
        case TokenType::STRING :
            value = new std::string{str};

            break;

        case TokenType::OP :
            // Find Operator index
            for (unsigned short i = 0; i < Operators.size(); i++)
                if (str == Operators[i])
                {
                    opIndex = (OperatorIndex)i;
                    break;
                }

            break;

        case TokenType::INTEGER_LITERAL :
            integer = std::stoi(str);

            break;

        default:
            type = TokenType::UNDEF;

            break;
        }
    }

    /*
        Constructor of SEPARATORs.
    */
    Token(TokenType tokenType, const char& c):
        type{tokenType}
    {
        switch (tokenType)
        {
        case TokenType::SEP :
            // Find Separator index
            for (unsigned short i = 0; i < Separators.size(); ++i)
                if (c == Separators[i])
                {
                    sepIndex = (SeparatorIndex)i;
                    break;
                }
                
            break;
        default:
            type = TokenType::UNDEF;

            break;
        }
    }

    /*
        KeyWord Contructor.
    */
    Token(KeyWordIndex kwInd) :
        type{TokenType::KEYWORD},
        kwIndex{kwInd}
    {
        // do nothing
    }

    /*
        Destructor.
    */
    ~Token()
    {
        if (type == TokenType::ID || type == TokenType::STRING)
            delete (std::string*)value;
    }
};

/*
    Template for Compiler Exception abstract class.
*/
template <class T>
class CompilerException
{
protected:
    char _buffer[maxLen];

    /*
        Get last line into the input position.
    */
    std::string
    getLastLine(std::string& input, size_t& pos) const
    {
        std::string auxLine;
        size_t i = pos - 1;
        size_t j = pos + 1;

        while(input[i-1] != '\n' && i > 0)
            i--;

        while(input[j] != '\n' && j < input.length())
            j++;
        
        for(;i < j; i++)
            auxLine += input[i];

        return auxLine;
    }

public:
    
    virtual void print(T&) const = 0;
};

#endif