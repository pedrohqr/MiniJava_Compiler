#include "scanner.h"

/* Constructor that receive a input file and store into the _input string. */
Scanner::Scanner(std::string FileName)
{
    _pos = 0;
    _line = _col = 1;

    //st = table;

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
        std::cerr << "Unable to open file" << std::endl;
}

size_t
Scanner::getLine() const
{
    return _line;
}

size_t
Scanner::getCol() const
{
    return _col;
}

/* Return true if the string is a KeyWord and the index relative on KeyWord array. */
bool
Scanner::isKeyword(const std::string& t, size_t& idx) const
{
    for(size_t i = 0; i < KeyWords.size(); i++)
        if (t == KeyWords[i])
        {
            idx = i;
            return true;
        }

    return false;
}

/* Return true if is separator. */
bool 
Scanner::isSeparator(const char& c) const
{   
    for (size_t i = 0; i < Separators.size(); i++)
        if (c == Separators[i])
            return true;
    
    return false;
}

/* Return true if the single char is a Operator. */
bool
Scanner::isOperator(const char& op) const
{
    for (size_t i = 0; i < Operators.size(); i++)
        if (op == (Operators[i])[0]) // if the first char is equal
            return true;

    return false;
}

/* Return true if the char is a letter or underscore. */
bool
Scanner::isLetter(const char& c) const
{
    return ( (c >= 65 && c <= 90) || c == '_' || (c >= 97 && c <= 122) ) ? true : false;
}

/* Return true if char c must be ignored. */
bool
Scanner::isIgnoredChars(const char& c)
{
    return ( c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f') ? true : false;
}

/* Return true if the char c is a digit. */
inline
bool
Scanner::isNumber(const char& c) const
{
    return (c >= 48 && c <= 57) ? true : false;
}

/* Return true if all string content is only numbers. */
bool 
Scanner::isNumber(const std::string& s) const
{
    size_t t = 0;
        
    while (s[t] != '\0')
        if (!isNumber(s[t])) // If isn't a number
            return false;
        else
            t++;

    return true;
}

/* 
    Advance in input position taking the columns and the lines.
*/
inline 
void 
Scanner::nextInputChar(int incPos = 1)
{
    if (_input[_pos] == '\n')
    {
        _col = 1;
        _line++;
    }
    else
        _col++;

    _pos += incPos;
}

/* 
    Return the next token from input text file. 
*/
std::shared_ptr<Token>
Scanner::nextToken()
try
{
    std::string lexeme;

    // Ignore white spaces and others
    _blankSpace:
        if ( isIgnoredChars(_input[_pos]) )
        {
            nextInputChar();
            goto _blankSpace;
        }
        else if ( _input[_pos] == '/' && _input[_pos+1] == '/' && _pos < _input.length()-2 ) // Line Comment
        {
            nextInputChar(2);
            goto _lineComment;
        }
        else if ( _input[_pos] == '/' && _input[_pos+1] == '*' && _pos < _input.length()-2 ) // Block Comment
        {
            nextInputChar(2);
            goto _blockComment;
        }
        else if ( isLetter(_input[_pos]) ) // Identifier
            goto _IDorKW;
        else if ( isNumber(_input[_pos]) ) // Integer Literal
            goto _IntLit;
        else if ( isSeparator(_input[_pos]) ) // Separator
        {
            nextInputChar(1);
            return std::make_shared<Token>(TokenType::SEP, _input[_pos-1]);
        }
        else if ( isOperator(_input[_pos]) ) // Operator
            goto _Operator;
        else if ( _input[_pos] == '"' ) // String
        {
            nextInputChar();
            goto _String;
        }
        else if ( _input[_pos] == '\0' ) // End of File
            return std::make_shared<Token>(TokenType::END_OF_FILE);
        else // Undefined Token
        {
            throw ScannerException("Undefined Token ""%c""",
                                    _input[_pos]);
        }


    // Ignore line comments (//)
    _lineComment:
        // if not find end of line
        if (_input[_pos] != '\n' && _pos < _input.length()-1)
        {
            nextInputChar();
            goto _lineComment;
        }
        else // found
            goto _blankSpace;

    // Ingore block comments (/**/)
    _blockComment:
        // if not find end of block comment
        if (!(_input[_pos] == '*' && _input[_pos+1] == '/'))
        {                
            nextInputChar();

            if (_pos >= _input.length()-2) // If comment block was not close
                throw ScannerException("Block comment '*/' not closed.");
            
            goto _blockComment;
        }
        else // found end of block comment
        {
            nextInputChar(2);
            goto _blankSpace;
        }

    // Identifiers and KeyWords
    _IDorKW:
        // If actual char is letter or underscore append to the lexeme
        if ( isLetter(_input[_pos]) || lexeme == "System" || lexeme == "System.out" || ( isNumber(_input[_pos]) && !isNumber(lexeme[0]) ) )
        {
            lexeme += _input[_pos];
            nextInputChar();
            goto _IDorKW;
        }
        else // when found other symbol, stop and verify if it's KeyWord or ID
            return std::make_shared<Token>(lexeme);

    // Strings
    _String:
        // If actual char isn't the only String Delimiter(not be double "" ), append String
        if ( ( _input[_pos] != '"' || (_input[_pos] == '"' && _input[_pos+1] == '"') ) && _pos < _input.length() )
        {
            lexeme += _input[_pos];

            if ( _input[_pos] == '"' && _input[_pos+1] == '"' )
                nextInputChar();

            nextInputChar();

            goto _String;
        }
        else if ( _pos >= _input.length() ) // String delimiter not closed
            throw ScannerException("String delimiter not closed("").");
        else // else, capture the lexeme value and store into the Token String
        {
            nextInputChar();
            return std::make_shared<Token>(TokenType::STRING, lexeme);
        }

    // Operators
    _Operator:
        if ((_input[_pos] == '&' && _input[_pos+1] == '&') || // &&
            (_input[_pos] == '=' && _input[_pos+1] == '=') || // ==
            (_input[_pos] == '!' && _input[_pos+1] == '=')    // !=
        )
        {
            nextInputChar(2);
            lexeme = _input[_pos-2] + _input[_pos-1];
            return std::make_shared<Token>(TokenType::OP, lexeme);
        }
        else // else, is a single operator
        {
            nextInputChar();
            lexeme = _input[_pos-1];
            return std::make_shared<Token>(TokenType::OP, lexeme);
        }
    
    // Integer Literals
    _IntLit:
        // If find a number, letter or underscore, append
        if ( isNumber(_input[_pos]) || isLetter(_input[_pos]) )
        {
            lexeme += _input[_pos];
            nextInputChar();
            goto _IntLit;
        }
        else if ( !isNumber(lexeme) ) // if lexeme is not a number, throw exception
            throw ScannerException("Cannot define name of a variable/funtion starting with a number.");
        else // if found another symbol, get the number
            return std::make_shared<Token>(TokenType::INTEGER_LITERAL, lexeme);
}
catch (ScannerException& E)
{
    E.print(*this);
    nextInputChar();
    return std::make_shared<Token>(TokenType::UNDEF);
}