#ifndef _PARSER_H
#define _PARSER_H

#include "scanner.h"
#include <memory>

class Parser
{
private:
	friend class ParserException;

	std::shared_ptr<Scanner> _scanner;
	std::shared_ptr<Token> 	 _lToken; // Lookahead Token

	inline void advance();
	bool seeNextToken(const TokenType&);
	inline void match(const TokenType&);
	inline void match(const KeyWordIndex&);
	inline void match(const SeparatorIndex&);
	inline void match(const OperatorIndex&);
	
public:
	Parser(std::string);
	~Parser();

	void run();

	void Program();
	void MainClass();
	void ClassDeclaration();
	void Statement();
	void Expression();
	void Op();
	void VarDeclaration();
	void Type();
	void MethodDeclaration();
	
	void _Expression();
	void __Expression();
};

class ParserException : protected CompilerException<Parser>
{
public:
	/*
        ParserException constructor.
    */
    ParserException(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
	
        vsnprintf(_buffer, maxLen, format, args);
    };

	/*
        Print method for parser class.
    */
    void
    print(Parser& parser) const
    {
        std::string lineStr{getLastLine(parser._scanner.get()->_input, parser._scanner.get()->_pos)};

        std::cerr << '\n';
        std::cerr << "\033[1;31mLexical Error [line: "<< parser._scanner.get()->_line <<"][column: " << parser._scanner.get()->_col << "]\033[0m: " << _buffer << std::endl;
        std::cerr << parser._scanner.get()->_line - 1  << " | ..." << std::endl;
        std::cerr << parser._scanner.get()->_line      << " | " << lineStr << std::endl;
        std::cerr << parser._scanner.get()->_line + 1  << " | ";
        for (size_t i = 0; i < parser._scanner.get()->_col-1; i++)
        {
            if (lineStr[i] == '\t')
                std::cerr << std::string(1, '\t');
            else
                std::cerr << std::string(1, ' ');
        }
        std::cerr << "^^^" << std::endl;
    }
};

#endif