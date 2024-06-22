PATH_CC?=/home/theo/Bureau/rpi/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
CCC?=$(PATH_CC)/arm-linux-gnueabihf-gcc

#definition des cibles par défaut
all:  data.o session.o libmcs.a

#compilation des fichiers source
data.o: data.c 
	gcc -c data.c

session.o: session.c 
	gcc -c session.c

#creation de la bibliotheque statique
libmcs.a: session.o data.o
	
	ar -qvs libmcs.a session.o data.o

clean: 
	rm -f *.o *.a

#[ -f libmcs.a ] && rm libmcs.a

#compilation croisée pour rpi de la lib mcs ainsi que du projet
cc: data.c session.c libmcs.a
	$(CCC) -c data.c
	$(CCC) -c session.c
	ar -qvs libmcs.a session.o data.o