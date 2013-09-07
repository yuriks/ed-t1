OBJS := VetorEquipe.o Equipe.o Torneio.o Torneio_io.o util.o xml.o SimuladorTorneio.o PilhaFonteCompetidor.o SimuladorPrinter.o

CXXFLAGS := -pedantic -Wall -Wextra -O0 -g3
LDFLAGS := -O0 -g3

.PHONY : all
all : main

.PHONY : clean
clean :
	$(RM) main *.o

main : $(OBJS)
