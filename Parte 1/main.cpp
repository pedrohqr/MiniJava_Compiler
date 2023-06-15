#include "scanner.h"

int main(int argc, char* argv[]) 
{
    if ( argc != 2 )
    {
        std::cout << "Use: ./" << argv[0] << " FILENAME.mj\n";
        return 1;
    }

    Scanner* scanner = new Scanner(argv[1]);
    
    Token* t;
    TokenType last = TokenType::END_OF_FILE;
    std::string* aux;

    do
    {
        t = scanner->nextToken();

        if(t->type == TokenType::ID)
            std::cout << "ID(\033[1;32m" << *(aux = (std::string*)t->value) << "\033[0m) ";
        else if(t->type == TokenType::KEYWORD)
            std::cout << "\033[1;32m" << KeyWords[ (int)t->kwIndex ] << "\033[0m ";
        else if(t->type == TokenType::OP)
            std::cout << "OPERATOR(\033[1;32m" << Operators[ (int)t->opIndex ] << "\033[0m) ";
        else if(t->type == TokenType::SEP)
            std::cout << "SEPARATOR(\033[1;32m" << Separators[ (int)t->sepIndex ] << "\033[0m) ";
        else if(t->type == TokenType::INTEGER_LITERAL)
            std::cout << "NUMBER(\033[1;32m" << t->integer << "\033[0m) ";
        else if(t->type == TokenType::STRING)
            std::cout << "STRING(\033[1;32m" << *(aux = (std::string*)t->value) << "\033[0m) ";
        else if(t->type == TokenType::END_OF_FILE)
            std::cout << "\033[1;33mEND_OF_FILE\033[0m ";
        else if(t->type == TokenType::UNDEF)
            std::cout << "UNDEFINED TOKEN( ) ";
        else
        {
            std::cerr << "TokenType not defined.\n";
            exit(1);
        }

        last = t->type;

        delete t;
    }while(last != TokenType::END_OF_FILE);

    std::cout << std::endl;

    delete scanner;
    
    return 0;
}
