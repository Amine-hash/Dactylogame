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