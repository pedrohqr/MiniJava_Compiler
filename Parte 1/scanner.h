#include <fstream>
#include "token.h"

class Scanner 
{
    private: 
        string input; // Armazena o texto de entrada
        int pos; // Posição atual
        int line; // Número da linha
        bool isKeyword(string t) const;

    public:

    //Construtor
        Scanner(string);

        int getLine();
    
        //Método que retorna o próximo token da entrada
        Token* nextToken();
    
        //Método para manipular erros
        void lexicalError(string);
};
