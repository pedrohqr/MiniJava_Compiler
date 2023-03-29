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

    //Método para palavras reservadas
bool Scanner::isKeyword(string t) const
{

    string KeyWords[19] = {"boolean", "class", "else", "extends", "false", "if", "int",
                            "lenght", "main", "new", "public", "return", "static", "String",
                            "System.out.println", "this", "true", "void", "while"};
    for(int i = 0; i < 19; i++)
    {
        if(t.compare(KeyWords[i]) == 0)
        {
            return true;
        }
    }

    return false;
}


//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    Token* tok;
    string lexeme;

    // Enquanto o caractere atual não for um espaço em branco, lê e concatena o token
    // TODO - Reconhecer: "asd//", "int*", "lenght()"
    while(input[pos] != ' ')
    {
        lexeme += input[pos];
        pos++;
    }
    cout << lexeme;
    //tok = new Token(0);

    // Cria novo token

    if (isKeyword(lexeme))
    {
         tok = new Token(KEYWORD);
         cout << "a" << endl;
    }
        
    //else if (isNumber(lexeme))
    
    //else if (isOp(lexeme))

    //else if (isSep(lexeme))

    //else if (isID(lexeme))

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


#########################################################################################################################################################
#include "scanner.h"  

// teste 9  

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

    //Método para palavras reservadas
bool Scanner::isKeyword(string t) const
{

    string KeyWords[19] = {"boolean", "class", "else", "extends", "false", "if", "int",
                            "lenght", "main", "new", "public", "return", "static", "String",
                            "System.out.println", "this", "true", "void", "while"};
    for(int i = 0; i < 19; i++)
    {
        if(t.compare(KeyWords[i]) == 0)
        {
            return true;
        }
    }

    return false;
}


//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    Token* tok;
    string lexeme;

    // Enquanto o caractere atual não for um espaço em branco, lê e concatena o token
    // TODO - Reconhecer: "asd//", "int*", "lenght()"
    while(input[pos] != ' ')
    {   
        /* Caso ele encontre um comentário feito pelo '//', ele vai lendo e incrementando a
        posição até a próxima quebra de linha*/
        if(input[pos] == '/' && input[pos+1] == '/' && pos < input.length())
        {
            while(input[pos] != '\n')
            {
                pos++;
            }
        }
        /* Caso ele encontre um comentário feito pela barra e estrela, ele vai lindo e incrementando a
        posição até encontrar o fechamento do comentário*/
        else if(input[pos] == '/' && input[pos+1] == '*' && pos < input.length())
        {
            while(input[pos] != '*' && input[pos+1] != '/')
            {
                pos++;
            }
        }
        /*Verificar se não é espaço em branco de acordo com a tabela ASCII:
        9 = \t
        10 = \n
        12 = \f
        13 = \r*/
        else if(input[pos] == 9 || input[pos] == 10 || input[pos] == 12 || input[pos] == 13)
        {
            ;
        }
        lexeme += input[pos];
        pos++;
    }
    cout << lexeme;
    //tok = new Token(0);

    // Cria novo token

    if (isKeyword(lexeme))
    {
         tok = new Token(KEYWORD);
         cout << "a" << endl;
    }
        
    //else if (isNumber(lexeme))
    
    //else if (isOp(lexeme))

    //else if (isSep(lexeme))

    //else if (isID(lexeme))

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
