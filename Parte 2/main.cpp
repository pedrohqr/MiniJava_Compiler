#include "parser.h"

int main(int argc, char* argv[])
{
    if ( argc != 2 )
    {
        std::cout << "Use: ./" << argv[0] << " FILENAME.mj\n";
        return EXIT_FAILURE;
    }    

    Parser* parser = new Parser(argv[1]);

    parser->run();
    
    std::cout << std::endl;

    delete parser;

    return EXIT_SUCCESS;
}