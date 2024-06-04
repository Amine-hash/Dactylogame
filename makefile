all : lib Projet

#la librairie lmcs est compilée dans le répertoire lib
Projet : Projet.c processus.c affichage.c conversion.c gestionFichier.c ./lib/libmcs.a
	gcc -o ProjetSrv processus.c affichage.c conversion.c gestionFichier.c Projet.c -DSERVEUR -L./lib -lmcs
	gcc -o ProjetClient processus.c affichage.c conversion.c gestionFichier.c Projet.c -L./lib -lmcs


clean : 
	rm -f *.o *.a ProjetSrv ProjetClient

#réalisation de la librairie à partir du makefile présent dans la librairie lib 
lib : 
	cd lib && make
	cp lib/libmcs.a ../