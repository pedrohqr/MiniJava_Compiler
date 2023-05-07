#include <fstream>
#include "token.h"
#include <string>
#include <memory>

class Scanner 
{
private: 
    std::string _input;
    size_t _pos;
    size_t _line, _col;

    bool isKeyword(const my::String&, size_t&) const;
    bool isSeparator(const unsigned char&) const;
    bool isOperator(const unsigned char&) const;
    bool isLetter(const unsigned char&) const;
    bool isIgnoredChars(const unsigned char&);
    inline bool isNumber(const unsigned char&) const;
    bool isNumber(const my::String&) const;

public:

    Scanner(std::string);

    int getLine();

    Token* nextToken();

    std::string getLastLine();

};
