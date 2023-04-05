#include <fstream>
#include "token.h"

class Scanner 
{
    private: 
        string input; // Armazena o texto de entrada
        size_t pos; // Posição atual
        size_t line; // Número da linha
        bool isKeyword(string t) const;
        bool isSep(unsigned char c) const;
        bool isOp(string& ret);
        bool isLetter(unsigned char c);
        bool ignoreChar(unsigned char c);
        bool isNumber(unsigned char c) const;
    public:

    //Construtor
        Scanner(string);

        int getLine();
    
        //Método que retorna o próximo token da entrada
        Token* nextToken();
    
        //Método para manipular erros
        void lexicalError(string);
};
