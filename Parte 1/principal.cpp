#include "scanner.h"

string Token_Names[7] = {"Undefined type",
                        "Identifier",
                        "Integer literal",
                        "Operator",
                        "Separator",
                        "KeyWord",
                        "End of file"};

int main(int argc, char* argv[]) 
{
    //Verifica se foi executado corretamente
    //Essa main espera receber o nome do arquivo a ser
    //executado na linha de comando.
    if ( argc != 2 )
    {
        cout << "Uso: ./compiler nome_arquivo.mj\n";
        return 1;
    }    

    Scanner* scanner = new Scanner(argv[1]);
    
    Token* t;
    
    do
    {
        t = scanner->nextToken();
        
        // Se o token for um ID
        if(t->name == ID)
            cout << "ID(" << t->lexeme << ") ";
        else if(t->name == KEYWORD) // Palavra reservada
            cout << t->lexeme << " ";
        else if(t->name == OP) // Operador
            cout << "OPERATOR(" << t->lexeme << ") ";
        else if(t->name == SEP) // Separador
            cout << "SEPARATOR(" << (char)t->attribute << ") ";
        else if(t->name == INTEGER_LITERAL)
            cout << "NUMBER(" << t->lexeme << ") ";
        else if(t->name == END_OF_FILE)
            cout << "END_OF_FILE ";
        else
            cout << "UNDEFINED TOKEN(" << t->lexeme << ") ";

    }while (t->name != END_OF_FILE);

    cout << endl;

    delete scanner;
    
    return 0;
}