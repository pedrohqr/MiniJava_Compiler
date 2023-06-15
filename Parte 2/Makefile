# Se o Sistema Operacional é Windows
ifeq ($(OS), Windows_NT)
	COMPILER_NAME = compiler.exe
else # Se não, é Unix
	COMPILER_NAME = compiler
endif

# Args
ARGS = -g -Wall

# Diretorios
OUTDIR = out
TMPDIR = tmp
TESTDIR = tests

OBJECTS = $(patsubst %.cpp, $(TMPDIR)/%.o, $(wildcard *.cpp))

# Compila e linka todos os arquivos e gera o executavel
all: path $(OUTDIR)/$(COMPILER_NAME)

path:
	@ mkdir -p $(OUTDIR)
	@ mkdir -p $(TMPDIR)

$(OUTDIR)/$(COMPILER_NAME): $(OBJECTS)
	g++ $(OBJECTS) -o $(OUTDIR)/$(COMPILER_NAME) $(ARGS)

$(TMPDIR)/scanner.o: scanner.cpp scanner.h token.h
	g++ -c scanner.cpp $(ARGS) -o $(TMPDIR)/scanner.o

$(TMPDIR)/main.o: main.cpp
	g++ -c main.cpp $(ARGS) -o tmp/main.o

$(TMPDIR)/parser.o: parser.cpp parser.h token.h
	g++ -c parser.cpp $(ARGS) -o $(TMPDIR)/parser.o

run:
	$(OUTDIR)/$(COMPILER_NAME) $(TESTDIR)/teste.mj

gdb:
	gdb --args $(OUTDIR)/$(COMPILER_NAME) $(TESTDIR)/teste.mj

val: #valgrind
	valgrind --leak-check=yes $(OUTDIR)/$(COMPILER_NAME) $(TESTDIR)/teste.mj

# Limpa arquivos objeto e o executavel
clean:
	rm -r $(TMPDIR)
	rm -r $(OUTDIR)
	clear