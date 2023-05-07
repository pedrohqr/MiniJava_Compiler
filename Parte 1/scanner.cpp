#include "scanner.h"
#include "exception.h"

/* Constructor that receive a input file and store into the _input string. */
Scanner::Scanner(std::string FileName)
{
    _pos = 0;
    _line = _col = 1;

    std::ifstream inputFile(FileName, std::ios::in);
    std::string _line;

    if (inputFile.is_open())
    {
        while (getline(inputFile,_line) )
        {
            _input.append(_line + '\n');
        }
        inputFile.close();
    }
    else 
        std::cout << "Unable to open file" << std::endl;
}

int
Scanner::getLine()
{
    return _line;
}

/* Get last line into the input position. */
std::string
Scanner::getLastLine()
{
    std::string auxLine;
    size_t i = _pos - 1;
    size_t j = _pos + 1;

    while(_input[i-1] != '\n' && i > 0)
        i--;

    while(_input[j] != '\n' && j < _input.length())
        j++;
    
    for(;i < j; i++)
        auxLine += _input[i];

    return auxLine;
}

/* Return true if the string is a KeyWord and the index relative on KeyWord array. */
bool
Scanner::isKeyword(const my::String& t, size_t& idx) const
{
    for(size_t i = 0; i < KWArrayLen; i++)
        if (t == KeyWords[i])
        {
            idx = i;
            return true;
        }

    return false;
}

/* Return true if is separator. */
bool 
Scanner::isSeparator(const unsigned char& c) const
{   
    for (size_t i = 0; i < SEPArrayLen; i++)
        if (c == Separators[i])
            return true;
    
    return false;
}

/* Return true if the single char is a Operator. */
bool
Scanner::isOperator(const unsigned char& c) const
{
    for (size_t i = 0; i < OPArrayLen; i++)
        if (c == Operators[i])
            return true;
    
    return false;
}

/* Return true if the char is a letter or underscore. */
bool
Scanner::isLetter(const unsigned char& c) const
{
    return ( (c >= 65 && c <= 90) || (c >= 97 && c <= 122) ) ? true : false;    
}

/* Return true if char c must be ignored. */
bool
Scanner::isIgnoredChars(const unsigned char& c)
{
    if (c == 10) // "\n"
    {
        _line++;
        _col = 1;
    }

    return (c == 9 || c == 10 || c == 12 || c == 13) ? true : false;
}

/* Return true if the char c is a digit. */
inline
bool
Scanner::isNumber(const unsigned char& c) const
{
    return (c >= 48 && c <= 57) ? true : false;
}

/* Return true if all string content is only numbers. */
bool 
Scanner::isNumber(const my::String& s) const
{
    size_t t = 0;
        
    while (s[t] != '\0')
        if (!isNumber(s[t])) // If isn't a number
            return false;
        else
            t++;

    return true;
}

/* Return the next token from input text file. */
Token*
Scanner::nextToken()
{
    Token* tok;
    my::String lexeme;
    size_t index = 0;

    // Ignore white spaces and others
    while(_input[_pos] == ' ' || isIgnoredChars(_input[_pos]))
    {
        _pos++;
        _col++;
    }

    // Ignore line comments (//)
    while(_input[_pos] == '/' && _input[_pos+1] == '/' && _pos < _input.length()-2)
    {
        while(_input[_pos] != '\n' && _pos < _input.length()-1)
        {
            _pos++;
            _col++;
        }
        if(_input[_pos] == '\n')
        {
            _line++;
            _col = 1;
            _pos++;
        }

        // Ignore white spaces and others
        while(_input[_pos] == ' ' || isIgnoredChars(_input[_pos]))
        {
            _pos++;
            _col++;
        }
    }

    // Ignore white spaces and others
    while(_input[_pos] == ' ' || isIgnoredChars(_input[_pos]))
    {
        _pos++;
        _col++;
    }

    // Ingore block comments (/**/)
    try
    {
        if(_input[_pos] == '/' && _input[_pos+1] == '*' && _pos < _input.length()-2)
        {
            _pos += 2;
            _col += 2;
            while(!(_input[_pos] == '*' && _input[_pos+1] == '/'))
            {
                if (_input[_pos] == 10) // "\n"
                {
                    _line++;
                    _col = 1;
                }
                if(_pos >= _input.length()-2) // If comment block was not close
                    throw ScannerException(_line, _col, "Block comment '*/' not closed.", getLastLine());
                _pos++;
                _col++;
            }

            if(_pos <= _input.length()-2)
            {
                _pos += 2;
                _col += 2;
            }
            if(_input[_pos] == '\n')
            {
                _line++;
                _col = 1;
                _pos++;
            }
        }
    }
    catch(ScannerException &E)
    {
        E.print();
    }

    // Ignore white spaces and others
    while(_input[_pos] == ' ' || isIgnoredChars(_input[_pos]))
    {
        _pos++;
        _col++;
    }

    /*****************************************************************/
    /*KEYWORD AND ID RECOGNIZEMENT*/

    try
    {
        // If the first char of the lexeme is a letter, then append letters and numbers
        if (isLetter(_input[_pos]) || _input[_pos] == '_')

        // While the actual char is a letter, underscore or number
        while( isLetter(_input[_pos]) || _input[_pos] == '_' || isNumber(_input[_pos]) || lexeme == "System" || lexeme == "System.out" )
        {
            _col++;
            lexeme.append(_input[_pos++]);
        }
        
        // If read another letter or underscore, then possible recognize a ID or KEYWORD
        if (!lexeme.isEmpty())
        {
            if(isKeyword(lexeme, index))
                tok = new TKeyWord(TokenType::KEYWORD, index);
            else
                tok = new TID(TokenType::ID, lexeme);
            
            return tok;
        }
    }
    catch(...)
    {
        ScannerException E(_line, _col, "Max size of a identifier is [" + std::to_string(my::MaxStringSize) + "].", getLastLine());
        E.print();
        return new TUndefToken(TokenType::UNDEF, '\0');
    }

    /*****************************************************************/
    /*STRING RECOGNIZEMENT*/

    if(_input[_pos] == '"')
    {
        std::string str;
        _pos++;
        _col++;
        while((_input[_pos] != '"' || (_input[_pos] == '"' && _input[_pos+1] == '"')) && _pos < _input.length())
        {
            if(_input[_pos] == '"' && _input[_pos+1] == '"')
            {
                str += _input[_pos++];
                _col++;
            }
            else
                str += _input[_pos];

            _pos++;
            _col++;
        }
        _pos++;
        _col++;

        if(!str.empty())
        {
            tok = new TString(TokenType::STRING, str);
            return tok;
        }
    }

    /*****************************************************************/
    /*SEPARATORS RECOGNIZEMENT*/
    if(isSeparator(_input[_pos]))
    {
        _col++;
        tok = new TSeparator(TokenType::SEP, _input[_pos++]);
        return tok;
    }
    /*****************************************************************/
    /*OPERATORS RECOGNIZEMENT*/
    if ((_input[_pos] == '&' && _input[_pos+1] == '&') || // &&
        (_input[_pos] == '=' && _input[_pos+1] == '=') || // ==
        (_input[_pos] == '!' && _input[_pos+1] == '=')    // !=
    )
    {
        lexeme += _input[_pos]; lexeme += _input[_pos+1]; // OTIMIZAR AQUI
        _pos += 2;
        _col += 2;
    }
    else if (isOperator(_input[_pos]))
    {
        lexeme.append(_input[_pos++]);
        _col++;
    }

    
    if(!lexeme.isEmpty())
    {
        tok = new TOperator(TokenType::OP, lexeme);
        return tok;
    }
    /*****************************************************************/
    /*NUMBER RECOGNIZEMENT*/
    while(isNumber(_input[_pos]) || isLetter(_input[_pos]) || _input[_pos] == '_')
    {
        lexeme += _input[_pos];
        _pos++;
        _col++;
    }
    try
    {
        if(!lexeme.isEmpty())
        {
            // If all chars are numeric digits
            if (isNumber(lexeme))
            {
                tok = new TNumber<unsigned int>(TokenType::INTEGER_LITERAL, lexeme.to_uint());
                return tok;
            }
            else // Else, is a invalid digit (started with a number)
                throw ScannerException(_line, _col-lexeme.length(), "Cannot define a name of a variable/funtion starting with a number.", getLastLine());
        }
    }
    catch(ScannerException& E)
    {
        E.print();
        tok = new TUndefToken(TokenType::UNDEF, '\0');
        return tok;
    }
    /*****************************************************************/
    /*END OF FILE RECOGNIZEMENT*/
    if(_input[_pos] == '\0') // END OF FILE
    {
        tok = new Token(TokenType::END_OF_FILE);
        return tok;
    }
    
    /*****************************************************************/
    /*IF IS NOT ANOTHER THESE SYMBOLS, RETURN UNDEFINED TOKEN*/
    ScannerException E(_line, _col, "Cannot define a name of a variable/funtion starting with a number", getLastLine());
    E.print();
    tok = new TUndefToken(TokenType::UNDEF, _input[_pos++]);
    _col++;

    return tok;
}
