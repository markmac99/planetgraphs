#
# This is an example Makefile for a countwords program.  This
# program uses both the scanner module and a counter module.
# Typing 'make' or 'make count' will create the executable file.
#

# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CC = gcc
CFLAGS  = -g -Wall

all: orbitcalcs cometpp pgsummary conjunctioncalcs

default: orbitcalcs

install: orbitcalcs cometpp pgsummary
	install -m 755 orbitcalcs /var/www/html/markmcintyreastro/planetgraphs/
	install -m 755 cometpp /var/www/html/markmcintyreastro/planetgraphs/
	install -m 755 pgsummary /var/www/html/markmcintyreastro/planetgraphs/
	install -m 755 conjunctioncalcs /var/www/html/markmcintyreastro/planetgraphs/
	
orbitcalcs: main.o DateMaths.o InitStaticData.o MagAndSize.o OrbitCalcs.o Perturbations.o Pluto.o TransitRiseSet.o Outputter.o Comets.o
	$(CC) $(CFLAGS) -o orbitcalcs main.o DateMaths.o InitStaticData.o MagAndSize.o OrbitCalcs.o Perturbations.o Pluto.o TransitRiseSet.o Outputter.o Comets.o -lstdc++ -lm

cometpp: cometmain.o DateMaths.o InitStaticData.o MagAndSize.o OrbitCalcs.o Perturbations.o Pluto.o TransitRiseSet.o Outputter.o Comets.o
	$(CC) $(CFLAGS) -o cometpp cometmain.o DateMaths.o InitStaticData.o MagAndSize.o OrbitCalcs.o Perturbations.o Pluto.o Comets.o TransitRiseSet.o Outputter.o -lstdc++ -lm

pgsummary: summarycalc.o DateMaths.o InitStaticData.o MagAndSize.o OrbitCalcs.o Perturbations.o Pluto.o TransitRiseSet.o Outputter.o Comets.o
	$(CC) $(CFLAGS) -o pgsummary summarycalc.o DateMaths.o InitStaticData.o MagAndSize.o OrbitCalcs.o Comets.o Perturbations.o Pluto.o TransitRiseSet.o Outputter.o -lstdc++ -lm

conjunctioncalcs: ConjunctionCalculator.o DateMaths.o InitStaticData.o MagAndSize.o OrbitCalcs.o Perturbations.o Pluto.o TransitRiseSet.o Outputter.o Comets.o
	$(CC) $(CFLAGS) -o conjunctioncalcs ConjunctionCalculator.o DateMaths.o InitStaticData.o MagAndSize.o OrbitCalcs.o Comets.o Perturbations.o Pluto.o TransitRiseSet.o Outputter.o -lstdc++ -lm

summarycalc.o: summarycalc.cpp OrbitCalcs.h Comets.h  
	$(CC) $(CFLAGS) -c summarycalc.cpp

main.o: main.cpp OrbitCalcs.h  
	$(CC) $(CFLAGS) -c main.cpp

cometmain.o: cometmain.cpp OrbitCalcs.h  Comets.h
	$(CC) $(CFLAGS) -c cometmain.cpp

DateMaths.o:  DateMaths.cpp OrbitCalcs.h
	$(CC) $(CFLAGS) -c DateMaths.cpp

Comets.o:  Comets.cpp OrbitCalcs.h
	$(CC) $(CFLAGS) -c Comets.cpp

InitStaticData.o:  InitStaticData.cpp OrbitCalcs.h
	$(CC) $(CFLAGS) -c InitStaticData.cpp

MagAndSize.o:  MagAndSize.cpp OrbitCalcs.h
	$(CC) $(CFLAGS) -c MagAndSize.cpp

Perturbations.o:  Perturbations.cpp OrbitCalcs.h
	$(CC) $(CFLAGS) -c Perturbations.cpp

OrbitCalcs.o:  OrbitCalcs.cpp OrbitCalcs.h
	$(CC) $(CFLAGS) -c OrbitCalcs.cpp

Pluto.o:  Pluto.cpp OrbitCalcs.h
	$(CC) $(CFLAGS) -c Pluto.cpp

TransitRiseSet.o:  TransitRiseSet.cpp OrbitCalcs.h
	$(CC) $(CFLAGS) -c TransitRiseSet.cpp

Outputter.o:  Outputter.cpp OrbitCalcs.h
	$(CC) $(CFLAGS) -c Outputter.cpp

ConjunctionCalculator.o: ConjunctionCalculator.cpp OrbitCalcs.h Comets.h  
	$(CC) $(CFLAGS) -c ConjunctionCalculator.cpp


# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) *.o *~
