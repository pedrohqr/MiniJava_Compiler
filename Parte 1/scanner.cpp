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
    while(input[pos] != (" "))
    {
        lexeme.append(input[pos]);
        pos++;
    }


    // Cria novo token

    if (isKeyword())

    else if (isID(lexeme))
        
    else if (isNumber(lexeme))
    
    else if (isOp(lexeme))

    else if (isSep(lexeme))

    else if ()

    // Retorna

    return tok;
 
}

void 
Scanner::lexicalError(string msg)
{
    cout << "Linha "<< line << ": " << msg << endl;
    
    exit(EXIT_FAILURE);
}
