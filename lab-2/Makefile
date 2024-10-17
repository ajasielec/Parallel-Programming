# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

# opcje optymalizacji:
# wersja do debugowania
# OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
OPT = -O3

# pliki naglowkowe
INC = -I../pomiar_czasu

# biblioteki
LIB = -L../pomiar_czasu -lpomiar_czasu -lm

# zaleznosci i komendy
fork: fork.o 
	$(LOADER) $(OPT) fork.o -o fork $(LIB)

# jak uzyskac plik fork.o ?
fork.o: fork.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) fork.c $(INC) 

# zaleznosci i komendy
clone: clone.o 
	$(LOADER) $(OPT) clone.o -o clone $(LIB)

# jak uzyskac plik clone.o ?
clone.o: clone.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) clone.c $(INC) 

# na 3.0:
multi_thread_clone: multi_thread_clone.o
	$(LOADER) $(OPT) multi_thread_clone.o -o multi_thread_clone $(LIB)

multi_thread_clone.o: multi_thread_clone.c
	$(CCOMP) -c $(OPT) multi_thread_clone.c $(INC)

# na 4.0:
zajecia2: zajecia2.o
	$(LOADER) $(OPT) zajecia2.o -o zajecia2 $(LIB)

zajecia2.o: zajecia2.c
	$(CCOMP) -c $(OPT) zajecia2.c $(INC)

all: fork clone multi_thread_clone zajecia2

clean:
	rm -f *.o fork clone multi_thread_clone zajecia2
