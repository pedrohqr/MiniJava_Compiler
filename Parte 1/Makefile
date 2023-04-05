# Se o Sistema Operacional é Windows
ifeq ($(OS), Windows_NT)
	COMPILER_NAME = compiler.exe
else # Se não, é Unix
	COMPILER_NAME = compiler
endif

# Diretorios
OUTDIR = out
TMPDIR = tmp

OBJECTS = $(patsubst %.cpp, $(TMPDIR)/%.o, $(wildcard *.cpp))

# Compila e linka todos os arquivos e gera o executavel
all: path $(OUTDIR)/$(COMPILER_NAME)

path:
	@ mkdir -p $(OUTDIR)
	@ mkdir -p $(TMPDIR)

$(OUTDIR)/$(COMPILER_NAME): $(OBJECTS)
	g++ $(OBJECTS) -o $(OUTDIR)/$(COMPILER_NAME)

$(TMPDIR)/scanner.o: scanner.cpp scanner.h token.h
	g++ -c scanner.cpp -Wall -o $(TMPDIR)/scanner.o

$(TMPDIR)/principal.o: principal.cpp
	g++ -c principal.cpp -Wall -o tmp/principal.o

run:
	$(OUTDIR)/$(COMPILER_NAME) teste.mj

# Limpa arquivos objeto e o executavel
clean:
	rm -f $(TMPDIR)/*.o
	rm -f $(OUTDIR)/$(COMPILER_NAME)