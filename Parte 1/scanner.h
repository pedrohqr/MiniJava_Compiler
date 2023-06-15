#include <fstream>
#include "token.h"
#include <string>
#include <memory>

class Scanner 
{
private: 
    std::string _input;
    size_t _line, _col;
    size_t _pos;

    bool isKeyword(const std::string&, size_t&) const;
    bool isSeparator(const char&) const;
    bool isOperator(const char&) const;
    bool isLetter(const char&) const;
    bool isIgnoredChars(const char&);
    inline bool isNumber(const char&) const;
    bool isNumber(const std::string&) const;
    inline void nextInputChar(int);

public:

    Scanner(std::string);

    size_t getLine();

    Token* nextToken();

    std::string getLastLine();

};
