# Se o Sistema Operacional é Windows
ifeq ($(OS), Windows_NT)
	COMPILER_NAME = compiler.exe
else # Se não, é Unix
	COMPILER_NAME = compiler
endif

# Compila e linka todos os arquivos e gera o executavel
all: scanner principal comp

scanner:
	g++ -c scanner.cpp -Wall -o scanner.o

principal:
	g++ -c principal.cpp -Wall -o principal.o

comp:
	g++ token.h principal.o scanner.o -Wall -o $(COMPILER_NAME)
	rm -f *.o

# Limpa arquivos objeto e o executavel
clean:
	rm -f *.o
	rm -f $(COMPILER_NAME)