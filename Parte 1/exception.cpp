#include "exception.h"

CompilerException::CompilerException(const std::string base_msg) : _base_msg(base_msg)
{

}

ScannerException::ScannerException(const size_t &lin, const size_t &col, const std::string base_msg, const std::string lineStr) : 
CompilerException(base_msg), _line(lin), _col(col), _lineString(lineStr)
{

}

void
ScannerException::print() const
{
    std::cerr << '\n';
    std::cerr << "\033[1;31mLexical Error [line: "<< _line <<"][column: " << _col << "]\033[0m: " << _base_msg << std::endl;
    std::cerr << _line - 1  << " | ..." << std::endl;
    std::cerr << _line      << " | " << _lineString << std::endl;
    std::cerr << _line + 1  << " | ";
    for (size_t i = 0; i < _col-1; i++)
    {
        if (_lineString[i] == '\t')
            std::cerr << std::string(1, '\t');
        else
            std::cerr << std::string(1, ' ');
    }
    std::cerr << "^^^" << std::endl;
}

/*ParserException::ParserException(const size_t &lin, const size_t &col, const std::string base_msg,  const std::string lineStr) :
CompilerException(base_msg), _line(lin), _col(col), _lineString(lineStr)
{

}*/