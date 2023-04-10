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
    //cout << this->input;

}

int
Scanner::getLine()
{
    return line;
}

    //Método para palavras reservadas
bool 
Scanner::isKeyword(string t) const
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

bool 
Scanner::isSep(unsigned char c) const
{   
    // ASCII
    if (c == 40 || // Parenteses esquerdo 
        c == 41 || // Parenteses direito
        c == 91 || // Colchetes esquerdo
        c == 93 || // Colchetes direito
        c == 123 ||// Chaves esquerda
        c == 125 ||// Chaves direita
        c == 59 || // Ponto e virgula
        c == 46 || // Ponto
        c == 44    // Virgula
        )
        return true;
    else
        return false;    
}

// Retorna se o caracter atual é um Operador e devolve o valor por referência
bool 
Scanner::isOp(string& ret)
{    
    if( input[pos] == 60 || // Menor
        input[pos] == 61 || // Igual
        input[pos] == 62 || // Maior
        input[pos] == 42 || // Asterisco
        input[pos] == 43 || // Mais
        input[pos] == 45 || // Menos
        input[pos] == 47 || // Divisao - barra
        input[pos] == 33    // Exclamação
        )
    {
        ret = input[pos++];
        return true;
    }
    else
        return false;

    // Se não chegou ao final do arquivo
    if(input.length() > pos)
    {
        if( (input[pos] == 38 && input[pos+1] == 38) || // "&&"
            (input[pos] == 61 && input[pos+1] == 61) || // "=="
            (input[pos] == 33 && input[pos+1] == 61)    // "!="
        )
        {
            ret = input[pos]+input[pos+1];
            pos += 2;
            return true;
        }
        else
            return false;
           
    }    
}

bool
Scanner::isLetter(unsigned char c)
{
    // Tabela ASCII
    if( c == 95 ||             // Se o caractere atual é underscore
       (c >= 65 && c <= 90) || // ou se for uma letra maiuscula
       (c >= 97 && c <= 122)   // ou se for uma letra minuscula
    )
        return true;
    else
        return false;
    
}

// Retorna true se o char atual é um dos caracteres a serem ignorados
bool
Scanner::isIgnoredChars(unsigned char c)
{
    if (c == 10) // "\n"
     line++;

    if(c == 9 || c == 10 || c == 12 || c == 13)
        return true;
    else
        return false;
}

// Retorna true se o char atual for um digito numerico
bool 
Scanner::isNumber(unsigned char c) const
{
    if(c >= 48 && c <= 57)
        return true;
    else
        return false;
}

// Percorre a string s e verifica se ela é um número
bool 
Scanner::isNumber(string s) const
{
    size_t t = 0;
    // Percorre a String
    while (s[t] != '\0')
        if (s[t] < 48 || s[t] > 57) // Se não for um número
            return false;
        else // Se for um número
            t++;

    return true;
    
}

//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    Token* tok;
    string lexeme;

    // Ignora os espaços em branco e derivados
    while(input[pos] == ' ' || isIgnoredChars(input[pos]))
        pos++;

    // Ignora comentarios //
    if(input[pos] == '/' && input[pos+1] == '/' && pos < input.length()-2)
    {
        while(input[pos] != '\n' && pos < input.length()-1)
            pos++;
        if(input[pos] == '\n')
        {
            line++;
            pos++;
        }
    }

    // Ingora comentarios /**/
    if(input[pos] == '/' && input[pos+1] == '*' && pos < input.length()-2)
    {
        pos += 2;
        while(input[pos] != '*' && input[pos+1] != '/' && pos < input.length()-2)
        {
            if (input[pos] == 10) // "\n"
                line++;
            pos++;
        }

        if(pos <= input.length()-2)
            pos += 2;
        if(input[pos] == '\n')
        {
            line++;
            pos++;
        }
    }

    // Ignora os espaços em branco e derivados
    while(input[pos] == ' ' || isIgnoredChars(input[pos]))
        pos++;

    /*****************************************************************/
    /*RECONHECIMENTO DE KEYWORD E ID*/

    // Enquanto o caractere atual for uma letra ou underscore
    while(isLetter(input[pos]) || lexeme == "System" || lexeme == "System.out" )
        lexeme += input[pos++];
    
    // Se leu alguma letra ou underscore, reconhece um possivel ID ou KEYWORD
    if (!lexeme.empty())
    {
        if(isKeyword(lexeme))        
            tok = new Token(KEYWORD, lexeme);
        else        
            tok = new Token(ID, lexeme);
        
        return tok;
    }

    /*****************************************************************/
    /*RECONHECIMENTO DE SEPARADORES*/
    if(isSep(input[pos]))
    {
        tok = new Token(SEP, (int)input[pos++]); // Guarda o atributo como o valor do char ASCII
        return tok;
    }
    /*****************************************************************/
    /*RECONHECIMENTO DE OPERADORES*/    
    if(isOp(lexeme))
    {
        tok = new Token(OP, lexeme);
        return tok;
    }
    /*****************************************************************/
    /*RECONHECIMENTO DE NUMEROS*/
    while(isNumber(input[pos]) || isLetter(input[pos]))
    {
        lexeme += input[pos];
        pos++;
    }
    if(!lexeme.empty())
    {
        // Se todos os caracteres forem dígitos numéricos
        if (isNumber(lexeme))
        {
            tok = new Token(INTEGER_LITERAL, lexeme);
            return tok;
        }
        else // Se não, é um ID inválido (começou com um número)
        {
            lexicalError("Cannot define a name of a variable/funtion starting with a number");
            tok = new Token(UNDEF, lexeme);
            return tok;
        }
    }
    /*****************************************************************/
    /*RECONHECE O FIM DO ARQUIVO*/
    if(input[pos] == '\0') // END OF FILE
    {
        tok = new Token(END_OF_FILE);
        return tok;
    }
    
    /*****************************************************************/
    /*SE NÃO FOR NENHUM DOS SIMBOLOS ANTERIORES, RETORNA INDEFINIDO*/
    tok = new Token(UNDEF, input[pos++]);
    //lexicalError("Unrecognized symbol");
    return tok;
}

void 
Scanner::lexicalError(string msg)
{
    cout << endl;
    cout << "\033[1;31mError - line "<< line << "\033[0m: " << msg << endl;
    
    //exit(EXIT_FAILURE);
}
