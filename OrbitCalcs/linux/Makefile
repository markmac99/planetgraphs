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

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: orbitcalcs

install: orbitcalcs
	install -m 755 orbitcalcs /var/www/html/planetgraphs/

# To create the executable file count we need the object files
#
orbitcalcs:  main.o DateMaths.o InitStaticData.o MagAndSize.o OrbitCalcs.o Perturbations.o Pluto.o TransitRiseSet.o Outputter.cpp
	$(CC) $(CFLAGS) -lm -o orbitcalcs main.o DateMaths.o InitStaticData.o MagAndSize.o OrbitCalcs.o Perturbations.o Pluto.o TransitRiseSet.o Outputter.cpp -lstdc++

# To create the object file countwords.o, we need the source
#
main.o: main.cpp OrbitCalcs.h  
	$(CC) $(CFLAGS) -c main.cpp

DateMaths.o:  DateMaths.cpp OrbitCalcs.h
	$(CC) $(CFLAGS) -c DateMaths.cpp

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

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) count *.o *~
