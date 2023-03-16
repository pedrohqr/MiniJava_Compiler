# Se o Sistema Operacional é Windows
ifeq ($(OS), Windows_NT)
	COMPILER_NAME = compiler.exe
else # Se não, é Unix
	COMPILER_NAME = compiler
endif

# Compila e linka todos os arquivos e gera o executavel
all: scanner principal comp

scanner:
	g++ -c scanner.cpp -Wall -o tmp/scanner.o

principal:
	g++ -c principal.cpp -Wall -o tmp/principal.o

comp:
	g++ token.h tmp/principal.o tmp/scanner.o -Wall -o out/$(COMPILER_NAME)
	rm -f *.o

# Limpa arquivos objeto e o executavel
clean:
	rm -f tmp/*.o
	rm -f out/$(COMPILER_NAME)