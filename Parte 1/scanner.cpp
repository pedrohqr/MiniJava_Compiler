#include "scanner.h"    

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input)
{
    /*this->input = input;
    cout << "Entrada: " << input << endl << "Tamanho: " 
         << input.length() << endl;*/
    pos = 0;
    line = 1;

    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile,line) )
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else 
        cout << "Unable to open file: " << strerror(errno) << endl; 

    //A próxima linha deve ser comentada posteriormente.
    //Ela é utilizada apenas para verificar se o 
    //preenchimento de input foi feito corretamente.
    cout << this->input;

}

int
Scanner::getLine()
{
    return line;
}

//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    Token* tok;
    string lexeme;

    // Enquanto o caractere atual não for um espaço em branco, lê e concatena o token
    // TODO - Reconhecer: "asd//", "int*", "lenght()"
    while(strcmp(&input[pos], " "))
    {
        lexeme = lexeme + input[pos];
        pos++;
    }
    tok = new Token(0);

    // Cria novo token

    if (isKeyword())
        // tok = new Token()
        
    else if (isNumber(lexeme))
    
    else if (isOp(lexeme))

    else if (isSep(lexeme))

    else if (isID(lexeme))

    else
        lexicalError("Unrecognized symbol");

    // Retorna

    return tok;
 
}

void 
Scanner::lexicalError(string msg)
{
    cout << "\033[1;31mError - line "<< line << "\033[0m: " << msg << endl;
    
    exit(EXIT_FAILURE);
}
