#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <iostream>

class CompilerException
{
protected:
    std::string _base_msg;
public:
    CompilerException(const std::string);
    virtual void print() const = 0;
};

class ScannerException : public CompilerException
{
private:
    size_t _line, _col;
    std::string _lineString;
public:
    ScannerException(const size_t&, const size_t&, const std::string,  const std::string);
    void print() const;
};

/*class ParserException : public CompilerException
{
private:
    size_t _line, _col;
    std::string _lineString;
public:
    ParserException(const size_t&, const size_t&, const std::string,  const std::string);
    void print() const;
};*/

#endif