#include "parser.h"

int main(int argc, char* argv[])
{
    if ( argc < 2 )
    {
        std::cout << "Use: ./" << argv[0] << " FILENAME1.mj FILENAME2.mj ... FILENAMEn.mj\n";
        return EXIT_FAILURE;
    }

    // Compile all files
    for (int i = 1; i < argc; i++)
    {
        Parser* parser = new Parser(argv[i]);

        parser->run();

        delete parser; 
    }

    return EXIT_SUCCESS;
}