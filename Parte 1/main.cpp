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
    
    do
    {
        t = scanner->nextToken();

        if(t->type == TokenType::ID)
            std::cout << "ID(" << ((TID*)t)->value << ") ";
        else if(t->type == TokenType::KEYWORD)
            std::cout << KeyWords[ ((TKeyWord*)t)->index ] << " ";
        else if(t->type == TokenType::OP)
            std::cout << "OPERATOR(" << ((TOperator*)t)->value << ") ";
        else if(t->type == TokenType::SEP)
            std::cout << "SEPARATOR(" << ((TSeparator*)t)->c << ") ";
        else if(t->type == TokenType::INTEGER_LITERAL)
            std::cout << "NUMBER(" << ((TNumber<int>*)t)->value << ") ";
        else if(t->type == TokenType::STRING)
            std::cout << "STRING(" << ((TString*)t)->value << ") ";
        else if(t->type == TokenType::END_OF_FILE)
            std::cout << "END_OF_FILE ";
        else if(t->type == TokenType::UNDEF)
            std::cout << "UNDEFINED TOKEN(" << ((TUndefToken*)t)->c << ") ";
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
