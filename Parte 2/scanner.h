#include <fstream>
#include "token.h"
#include <memory>
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <stdexcept>

class Scanner 
{
private: 
    friend class ScannerException;
    friend class ParserException;
    friend class Parser;

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

    size_t getLine() const;

    size_t getCol() const;

    std::shared_ptr<Token> nextToken();
};

class ScannerException : protected CompilerException<Scanner>
{
public:
    /*
        ScannerException constructor.
    */
    ScannerException(const char* format, ...)
    {
        va_list args;
        va_start(args, format);

        vsnprintf(_buffer, maxLen, format, args);
    }

    /*
        Print method for scanner class.
    */
    void
    print(Scanner& scanner) const
    {
        std::string lineStr{getLastLine(scanner._input, scanner._pos)};

        std::cerr << '\n';
        std::cerr << "\033[1;31mLexical Error [line: "<< scanner._line <<"][column: " << scanner._col << "]\033[0m: " << _buffer << std::endl;
        std::cerr << scanner._line - 1  << " | ..." << std::endl;
        std::cerr << scanner._line      << " | " << lineStr << std::endl;
        std::cerr << scanner._line + 1  << " | ";
        for (size_t i = 0; i < scanner._col-1; i++)
        {
            if (lineStr[i] == '\t')
                std::cerr << std::string(1, '\t');
            else
                std::cerr << std::string(1, ' ');
        }
        std::cerr << "^^^" << std::endl;
    }
};