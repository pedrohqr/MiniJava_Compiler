#include "parser.h"
#include <cstdarg>

Parser::Parser(std::string input) :
	filename{input},
	errors{0}
{
	_currentST = _globalST = new SymbolTable();
	initSimbolTable();
	_scanner = std::make_shared<Scanner>(input);
}

Parser::~Parser()
{
	delete _globalST;
	//delete _currentST;	
}

inline
void
Parser::advance()
{
	// Assign next token
	_lToken = _scanner->nextToken();
}

inline
void
Parser::match(const TokenType& expected)
try
{
	if ( _lToken->type == expected || _lToken->type == TokenType::UNDEF )
	{
		if (_lToken->type == TokenType::ID) // If it's an identifier
		{
			std::string *id = (std::string*)_lToken->value; // Get string id by pointers
			if ( !_currentST->find( *id ) ) // if not find an entry in symbol table, throw
			{
				throw ParserException("Identifier not defined: (%s)",
									id->c_str());
			}
		}
		advance();
	}
	else if ( _lToken->type == TokenType::END_OF_FILE )
	{
		exit(0);
	}
	else
		throw ParserException("Unexpected token: You give (%s) and the expected was (%s).", 
							  	TokenTypeNames[(int)_lToken->type+1],
							  	TokenTypeNames[(int)expected+1]);
}
catch (ParserException& E)
{
	E.print(*this);
	errors++;
	advance();
}

inline
void
Parser::match(const KeyWordIndex& expected)
try
{
	if ( _lToken->kwIndex == expected )
		advance();
	else if ( _lToken->type == TokenType::ID )
	{
		std::string *id = (std::string*)_lToken->value; // Get string id by pointers
		throw ParserException("Unexpected keyword: You give (%s) and the expected was (%s).", 
							  	id->c_str(),
							  	KeyWords[(int)expected]);
	}
	else
		throw ParserException("Unexpected keyword: You give (%s) and the expected was (%s).", 
							  	KeyWords[(int)_lToken->kwIndex],
							  	KeyWords[(int)expected]);
}
catch (ParserException& E)
{
	E.print(*this);
	errors++;
	advance();
}

inline
void
Parser::match(const SeparatorIndex& expected)
try
{
	if ( _lToken->sepIndex == expected )
		advance();
	else if ( _lToken->type != TokenType::SEP )
		throw ParserException("Unexpected separator: You give an (%s) and the expected was (%c).", 
							  	TokenTypeNames[(int)_lToken->type],
							  	Separators[(int)expected]);
	else
		throw ParserException("Unexpected separator: You give (%c) and the expected was (%c).", 
							  	Separators[(int)_lToken->sepIndex],
							  	Separators[(int)expected]);
}
catch (ParserException& E)
{
	E.print(*this);
	errors++;
	advance();
}

inline
void
Parser::match(const OperatorIndex& expected)
try
{
	if ( _lToken->opIndex == expected )
		advance();
	else
		throw ParserException("Unexpected operator: You give (%s) and the expected was (%s).", 
							  	Operators[(int)_lToken->opIndex],
							  	Operators[(int)expected]);
}
catch (ParserException& E)
{
	E.print(*this);
	errors++;
	advance();
}

/*
	Add to current Symbol Table. If the current lookahead token its an identifier,
	then add to the current symbol table.
*/
inline
void
Parser::addToCurrST()
try
{
	if (_lToken->type == TokenType::ID)
		if (!_currentST->add(new STEntry(_lToken.get())))
		{
			std::string *ptr = (std::string*)_lToken->value;
			throw ParserException("Identifier already exists on current scope: (%s).",
								  ptr->c_str());
		}
}
catch (ParserException &E)
{
	E.print(*this);
	errors++;
}

/*
	Add to global Symbol Table. If the current lookahead token its an identifier,
	then add to the global symbol table.
*/
inline
void
Parser::addToGlobalST()
try
{
	if (_lToken->type == TokenType::ID)
		if (!_globalST->add(new STEntry(_lToken.get())))
		{
			std::string *ptr = (std::string*)_lToken->value;
			throw ParserException("Identifier already exists on global scope: (%s).",
								  ptr->c_str());
		}
}
catch (ParserException &E)
{
	E.print(*this);
	errors++;
}

/*
	Create new Symbol Table.
*/
inline
void
Parser::createNewST()
{
	SymbolTable* newST = new SymbolTable(_currentST);
	_currentST = newST;
}

/*
	Create new Symbol Table with parent.
*/
inline
void
Parser::createNewST(SymbolTable* parent)
{
	SymbolTable* newST = new SymbolTable(parent);
	_currentST = newST;
}

/*
	Close current Symbol Table. This method free the current symbol table and forces
	the current point to the parent symbol table.
*/
inline
void
Parser::closeCurrST()
{
	SymbolTable* tmp = _currentST->getParent();
	delete _currentST;
	_currentST = tmp;
}

/*
	Initialization of Symbol Table.
*/
inline
void
Parser::initSimbolTable()
{	
	/*_globalST->add( new STEntry( new Token(KeyWordIndex::CLASS), true ) );
	_globalST->add( new STEntry( new Token(KeyWordIndex::EXTENDS), true ) );
	_globalST->add( new STEntry( new Token(KeyWordIndex::PUBLIC), true ) );
	_globalST->add( new STEntry( new Token(KeyWordIndex::INT), true ) );
	_globalST->add( new STEntry( new Token(KeyWordIndex::NEW), true ) );
	_globalST->add( new STEntry( new Token(KeyWordIndex::STATIC), true ) );
	_globalST->add( new STEntry( new Token(KeyWordIndex::STRING), true ) );
	_globalST->add( new STEntry( new Token(KeyWordIndex::THIS), true ) );
	_globalST->add( new STEntry( new Token(KeyWordIndex::VOID), true ) );*/
}

void
Parser::run()
{
	std::cout << "************* compilation started **************" << std::endl;
	std::cout << "File: " << filename << std::endl;

	advance();

	Program();

	std::cout << "Total errors: " << errors << std::endl;
	std::cout << "************* compilation finished *************" << std::endl;
}

void
Parser::Program()
{
	// 1. Program -> MainClass (ClassDeclaration)* EOF
    MainClass();
	while(_lToken->kwIndex == KeyWordIndex::CLASS)
		ClassDeclaration();
	match(TokenType::END_OF_FILE);
}

void
Parser::MainClass()
{
    // 2. MainClass -> class ID {public static void main(String[] ID){ Statement }}
	match(KeyWordIndex::CLASS);
	addToGlobalST();
	match(TokenType::ID);
	createNewST();
	match(SeparatorIndex::LBRACE);
	match(KeyWordIndex::PUBLIC); 
	match(KeyWordIndex::STATIC); 
	match(KeyWordIndex::VOID); 
	match(KeyWordIndex::MAIN); 
	match(SeparatorIndex::LPAREN); 
	match(KeyWordIndex::STRING);
	match(SeparatorIndex::LBRACKET);
	match(SeparatorIndex::RBRACKET);
	addToCurrST();
	match(TokenType::ID);
	match(SeparatorIndex::RPAREN);
	match(SeparatorIndex::LBRACE);
	Statement(); 
	match(SeparatorIndex::RBRACE);
	closeCurrST();
	match(SeparatorIndex::RBRACE);
}

void
Parser::ClassDeclaration()
{
	// 3. ClassDeclaration -> class ID (extends ID)? { (VarDeclaration)* (MethodDeclaration)* }
	match(KeyWordIndex::CLASS);

	addToGlobalST();

	match(TokenType::ID);
	if (_lToken->kwIndex == KeyWordIndex::EXTENDS)
	{
		match(KeyWordIndex::EXTENDS);
		match(TokenType::ID);
		createNewST(_currentST);
	}
	else
		createNewST();

	match(SeparatorIndex::LBRACE);

	// FIRST(VarDeclaration) = FIRST(Type) = {int, boolean, ID}
	while(_lToken->kwIndex == KeyWordIndex::INT || _lToken->kwIndex == KeyWordIndex::BOOLEAN || _lToken->type == TokenType::ID)
		VarDeclaration();
	
	// FIRST(MethodDeclaration) = {public}
	while(_lToken->kwIndex == KeyWordIndex::PUBLIC)
		MethodDeclaration();
	
	match(SeparatorIndex::RBRACE);
		
	//closeCurrST();
}

void
Parser::VarDeclaration()
{
	// 4. VarDeclaration -> Type ID;
	
	Type(); 
	addToCurrST();
	match(TokenType::ID); 
	match(SeparatorIndex::SEMICOLON);
}

void
Parser::MethodDeclaration()
{
	// 5. MethodDeclaration -> public Type ID( (Type ID(, Type ID)*)? ){ (VarDeclaration)* (Statement)* return Expression; }

	match(KeyWordIndex::PUBLIC);
	Type();
	addToCurrST();
	match(TokenType::ID);
	match(SeparatorIndex::LPAREN);

	createNewST();

	if(_lToken->sepIndex != SeparatorIndex::RPAREN)
	{
		Type();
		addToCurrST();
		match(TokenType::ID);
		while(_lToken->sepIndex != SeparatorIndex::RPAREN)
		{
			match(SeparatorIndex::COMMA);
			Type();
			addToCurrST();
			match(TokenType::ID);
		}
	}

	match(SeparatorIndex::RPAREN);
	match(SeparatorIndex::LBRACE);

	// FIRST(VarDeclaration) = FIRST(Type) = {int, boolean, ID}
	// Enquanto lToken = int ou boolean -> (VarDeclaration)
	while(_lToken->kwIndex == KeyWordIndex::INT || _lToken->kwIndex == KeyWordIndex::BOOLEAN || (_lToken->type == TokenType::ID && seeNextToken(TokenType::ID)))
	{
		/*Type();
		match(TokenType::ID);
		match(SeparatorIndex::SEMICOLON);*/
		VarDeclaration();
	}

	// FIRST(Statement) = { {, if, while, System.out.println, ID}

	while(_lToken->kwIndex != KeyWordIndex::RETURN)
		Statement();

	match(KeyWordIndex::RETURN);
	Expression();
	match(SeparatorIndex::SEMICOLON);

	match(SeparatorIndex::RBRACE);

	closeCurrST();
}

void
Parser::Type()
{
	/*
	6. Type -> int[] | boolean | int | ID
	Obs: Necessário a reescrita de produção para (int).
	_int -> [] | ø
	*/ 

	if(_lToken->kwIndex == KeyWordIndex::BOOLEAN)
		match(KeyWordIndex::BOOLEAN);
	else if(_lToken->type == TokenType::ID)
		match(TokenType::ID);
	else if (_lToken->kwIndex == KeyWordIndex::INT)
	{
		match(KeyWordIndex::INT);
		if(_lToken->sepIndex == SeparatorIndex::LBRACKET)
		{
			match(SeparatorIndex::LBRACKET);
			match(SeparatorIndex::RBRACKET);
		}
	}
}

void
Parser::Statement()
try
{
	/*

	7. Statement -> { (Statement)* }
	| if( Expression ) Statement else Statement
	| while ( Expression ) Statement
	| System.out.println( Expression );
	| ID = Expression ;
	| ID[ Expression ] = Expression ;

	Obs: A produção ID causa recursão. Logo é necessário fazer uma quebra de ID em duas produções

	_Id -> = Expression;
	| [ Expression ] = Expression;

	*/

	if(_lToken->sepIndex == SeparatorIndex::LBRACE && _lToken->type == TokenType::SEP)
	{
		match(SeparatorIndex::LBRACE); 
		while(_lToken->sepIndex != SeparatorIndex::RBRACE)
			Statement();
		match(SeparatorIndex::RBRACE);
	}
	else if(_lToken->kwIndex == KeyWordIndex::IF && _lToken->type == TokenType::KEYWORD)
	{
		match(KeyWordIndex::IF); match(SeparatorIndex::LPAREN); Expression(); match(SeparatorIndex::RPAREN);
			Statement();
		match(KeyWordIndex::ELSE);
			Statement();
	}
	else if(_lToken->kwIndex == KeyWordIndex::WHILE && _lToken->type == TokenType::KEYWORD)
	{
		match(KeyWordIndex::WHILE); match(SeparatorIndex::LPAREN); Expression(); match(SeparatorIndex::RPAREN);
			Statement();
	}
	else if(_lToken->kwIndex == KeyWordIndex::SYSOUT && _lToken->type == TokenType::KEYWORD) // System.out.println
	{
		match(KeyWordIndex::SYSOUT); match(SeparatorIndex::LPAREN); Expression(); match(SeparatorIndex::RPAREN); match(SeparatorIndex::SEMICOLON);
	}
	else if(_lToken->type == TokenType::ID)
	{
		match(TokenType::ID);
		if(_lToken->opIndex == OperatorIndex::ATRIB && _lToken->type == TokenType::OP) // Se "="
		{
			match(OperatorIndex::ATRIB); 
			Expression(); 
			match(SeparatorIndex::SEMICOLON);
		}
		else// "["
		{
			match(SeparatorIndex::LBRACKET); Expression(); match(SeparatorIndex::RBRACKET); match(OperatorIndex::ATRIB); Expression(); match(SeparatorIndex::SEMICOLON);
		}		
	}
	else //if(_lToken->type == TokenType::UNDEF) 
	{	
		advance();
		throw ParserException("Unexpected token (%s).",
								TokenTypeNames[(int)_lToken->type]);
	}
}
catch(ParserException &E)
{
	E.print(*this);
	errors++;
}


void
Parser::Expression()
try
{
	/*
	
	8. Expression -> Expression Op Expression
	| Expression [ Expression ]
	| Expression . length
	| Expression . ID ( (Expression(, Expression)*)? )
	| INTEGER_LITERAL
	| true
	| false
	| ID
	| this
	| new int [ Expression ]
	| new ID()
	| STRING
	| ! Expression
	| ( Expression )

	Obs: As subproduções (Expression), (Expression.) e (new) necessitam ser reescritas para resolver a recursão à esquerda.

	Expression -> INTEGER_LITERAL _Expression
	| true _Expression
	| false _Expression
	| ID _Expression
	| this _Expression
	| new int [ Expression ] _Expression
	| new ID() _Expression
	| STRING _Expression
	| ! Expression _Expression
	| ( Expression ) _Expression

	_Expression -> Op Expression _Expression
	| [ Expression ] _Expression
	| . length _Expression
	| . ID ( (Expression(, Expression)*)? ) _Expression
	| ø

	_new -> int [ Expression ]
	| ID()

	__Expression -> length
	| ID ( (Expression(, Expression)*)? )

	*/
	if(_lToken->type == TokenType::INTEGER_LITERAL)
	{	
		match(TokenType::INTEGER_LITERAL);
		_Expression();
	}
	else if(_lToken->kwIndex == KeyWordIndex::TRUE)
	{	
		match(KeyWordIndex::TRUE);
		_Expression();
	}
	else if(_lToken->kwIndex == KeyWordIndex::FALSE)
	{
		match(KeyWordIndex::FALSE);	
		_Expression();
	}
	else if(_lToken->type == TokenType::ID)
	{
		match(TokenType::ID);	
		_Expression();
	}
	else if(_lToken->kwIndex == KeyWordIndex::THIS)
	{
		match(KeyWordIndex::THIS);	
		_Expression();
	}
	else if(_lToken->kwIndex == KeyWordIndex::NEW)
	{
		match(KeyWordIndex::NEW);
		if(seeNextToken(TokenType::ID)) // se próximo token for ID
		{
			match(TokenType::ID); 
			match(SeparatorIndex::LPAREN); 
			match(SeparatorIndex::RPAREN);
		}
		else // próximo token for int
		{
			match(KeyWordIndex::INT); 
			match(SeparatorIndex::LBRACKET); 
			Expression(); 
			match(SeparatorIndex::RBRACKET);
		}
		_Expression();
	}
	else if(_lToken->type == TokenType::STRING)
	{
		match(TokenType::STRING); 	
		_Expression();
	}
	else if(_lToken->opIndex == OperatorIndex::NOT)
	{
		match(OperatorIndex::NOT); 
		Expression(); 	
		_Expression();
	}
	else if(_lToken->sepIndex == SeparatorIndex::LPAREN)
	{
		match(SeparatorIndex::LPAREN); 
		Expression(); 
		match(SeparatorIndex::RPAREN); 
		_Expression();
	}
	else
	{
		if(_lToken->type == TokenType::SEP)
			throw ParserException("Unexpected token: You give (%c) and the expected was a valid expression.",
								  Separators[(int)_lToken->sepIndex]);
		else
			throw ParserException("Unexpected token: You give (%s) and the expected was a valid expression.",
								  TokenTypeNames[(int)_lToken->type]);
	}
}
catch (ParserException &E)
{
	E.print(*this);
	errors++;
}

void
Parser::Op()
{
	//	9. Op -> && | < | > | == | != | + | - | * | /

	if(_lToken->opIndex == OperatorIndex::AND)
		match(OperatorIndex::AND);
	else if(_lToken->opIndex == OperatorIndex::LESS)
		match(OperatorIndex::LESS);
	else if(_lToken->opIndex == OperatorIndex::GRATER)
		match(OperatorIndex::GRATER);
	else if(_lToken->opIndex == OperatorIndex::EQUALS)
		match(OperatorIndex::EQUALS);
	else if(_lToken->opIndex == OperatorIndex::NEQUAL)
		match(OperatorIndex::NEQUAL);
	else if(_lToken->opIndex == OperatorIndex::PLUS)
		match(OperatorIndex::PLUS);
	else if(_lToken->opIndex == OperatorIndex::MINUS)
		match(OperatorIndex::MINUS);
	else if(_lToken->opIndex == OperatorIndex::STAR)
		match(OperatorIndex::STAR);
	else if(_lToken->opIndex == OperatorIndex::BAR)
		match(OperatorIndex::BAR);
}

void
Parser::_Expression()
try
{
	/*
	_Expression -> Op Expression _Expression
	| [ Expression ] _Expression
	| . length _Expression
	| . ID ( (Expression(, Expression)*)? ) _Expression
	| ø
	*/
	if(_lToken->type == TokenType::OP)
	{
		Op(); 
		Expression(); 
		_Expression();
	}
	if(_lToken->sepIndex == SeparatorIndex::LBRACKET)
	{
		match(SeparatorIndex::LBRACKET); 
		Expression(); 
		match(SeparatorIndex::RBRACKET); 
		_Expression();
	}
	else if(_lToken->sepIndex == SeparatorIndex::PERIOD)
	{
		match(SeparatorIndex::PERIOD); 
		__Expression(); 
		_Expression();
	}
	else
		;
}
catch(ParserException &E)
{
	E.print(*this);
	errors++;
}

void
Parser::__Expression()
{
	/*
	__Expression -> length
	| ID ( (Expression(, Expression)*)? )
	*/

	if(_lToken->kwIndex == KeyWordIndex::LENGTH)
	{
		match(KeyWordIndex::LENGTH);
	}
	else if(_lToken->type == TokenType::ID)
	{
		match(TokenType::ID); 
		match(SeparatorIndex::LPAREN);

		// Se a chamada de função não tem argumentos
		if(_lToken->sepIndex == SeparatorIndex::RPAREN)
			match(SeparatorIndex::RPAREN);
		else
		{
			Expression();
			
			while (_lToken->sepIndex != SeparatorIndex::RPAREN)
			{
				match(SeparatorIndex::COMMA); 
				Expression();
			}
			
			match(SeparatorIndex::RPAREN);
		}		
	}
}

/* 
	Olha para o próximo token do lookahead mas não avança o ponteiro. Retorna se o tipo
	esperado é compatível com o atual.
*/
bool
Parser::seeNextToken(const TokenType& expected)
{
	size_t pos = _scanner.get()->_pos; // Guarda posição antes do ponteiro avançar
	// Cria um SmartPtr para receber o próximo token e ser destruído automaticamente ao final da função
	std::shared_ptr<Token> next(_scanner->nextToken());

	// Volta a posição inicial
	_scanner.get()->_pos = pos;

	return next->type == expected ? true : false;	
}