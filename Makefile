CC=cc
CFLAGS=-g3 `llvm-config --cflags`

LD=c++
LLVMCONFIG=`llvm-config --cxxflags --ldflags --libs core executionengine mcjit interpreter analysis native bitwriter --system-libs`
LDFLAGS=$(LLVMCONFIG) -lfl -lm

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
HSRC=$(wildcard *.h)

all: bison flex calculator

calculator: $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

bison: calculator.y
	bison -d calculator.y

flex: calculator.l
	flex calculator.l

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< 

backend.o: backend.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o calculator *~
