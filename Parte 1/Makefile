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

OBJECTS = $(patsubst %.cpp, $(TMPDIR)/%.o, $(wildcard *.cpp))

# Compila e linka todos os arquivos e gera o executavel
all: path $(OUTDIR)/$(COMPILER_NAME)

path:
	@ mkdir -p $(OUTDIR)
	@ mkdir -p $(TMPDIR)

$(OUTDIR)/$(COMPILER_NAME): $(OBJECTS)
	g++ $(OBJECTS) -o $(OUTDIR)/$(COMPILER_NAME)

$(TMPDIR)/scanner.o: scanner.cpp scanner.h token.h
	g++ -c scanner.cpp $(ARGS) -o $(TMPDIR)/scanner.o

$(TMPDIR)/main.o: main.cpp
	g++ -c main.cpp $(ARGS) -o tmp/main.o

$(TMPDIR)/String.o: String.cpp String.h
	g++ -c String.cpp $(ARGS) -o $(TMPDIR)/String.o

$(TMPDIR)/exception.o: exception.cpp exception.h
	g++ -c exception.cpp $(ARGS) -o $(TMPDIR)/exception.o

run:
	$(OUTDIR)/$(COMPILER_NAME) teste.mj

gdb:
	gdb --args $(OUTDIR)/$(COMPILER_NAME) teste.mj

val: #valgrind
	valgrind --leak-check=yes $(OUTDIR)/$(COMPILER_NAME) teste.mj

# Limpa arquivos objeto e o executavel
clean:
	rm -r $(TMPDIR)
	rm -r $(OUTDIR)