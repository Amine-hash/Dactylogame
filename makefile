PATH_CC=~/Documents/IG2I/IG2I/LA1/Linux_embarqué_2023-2024/Module_2/src/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
CCC=$(PATH_CC)/arm-linux-gnueabihf-gcc
DESTDIR=/home/oem/Documents/IG2I/IG2I/LA1/Objets_connectés/target_rpi
PI=192.168.1.31
PI2=192.168.1.31
MON_PATH=/home/oem/Documents/IG2I/IG2I/LA1/Linux_embarqué_2023-2024/Module_2
all : lib Projet cc

#la librairie lmcs est compilée dans le répertoire lib
Projet : Projet.c processus.c affichage.c conversion.c gestionFichier.c
	cd lib && make clean
	cd lib && make
	gcc -o serveur_pc processus.c affichage.c conversion.c gestionFichier.c Projet.c -DSERVEUR -L./lib -lmcs -lpthread -lncurses
	gcc -o client_pc processus.c affichage.c conversion.c gestionFichier.c Projet.c -L./lib -lmcs -lpthread -lncurses
clean : 
	rm -f *.o *.a ProjetSrv ProjetClient serveur_rpi client_rpi serveur_pc client_pc

#réalisation de la librairie à partir du makefile présent dans la librairie lib 
lib :
	cd lib && make clean
	cd lib && make
	cp lib/libmcs.a ../

#compilation croisée pour rpi de la lib mcs ainsi que du projet

cc : Projet.c processus.c affichage.c conversion.c gestionFichier.c ./lib/libmcs.a
	echo $(DESTDIR)
	make ccLib
	$(CCC) -DSERVEUR -I$(DESTDIR)/include -L$(DESTDIR)/lib -L$(MON_PATH)/target_rpi/lib -I$(MON_PATH)/target_rpi/include  -I$(MON_PATH)/target_rpi/include/ncurses -L./lib -std=gnu99 -o serveur_rpi affichage.c processus.c conversion.c gestionFichier.c Projet.c -lmcs -lpthread -lncurses
	$(CCC) -L./lib  -DCROSS_COMPILE -L$(MON_PATH)/target_rpi/lib -I$(MON_PATH)/target_rpi/include  -I$(MON_PATH)/target_rpi/include/ncurses -I$(DESTDIR)/include -L$(DESTDIR)/lib -std=gnu99 -o client_rpi processus.c affichage.c conversion.c gestionFichier.c Projet.c fonctionWiringPi.c -lmcs -lpthread -lwiringPi -lwiringPiDev -lncurses
	sshpass -prpi scp serveur_rpi  pi@$(PI):/home/pi/objet_connecte
	sshpass -prpi scp client_rpi pi@$(PI):/home/pi/objet_connecte
	sshpass -prpi scp -r ./dico pi@$(PI):/home/pi/objet_connecte
	sshpass -prpi scp -r ./lib pi@$(PI):/home/pi/objet_connecte
#cp  $MON_PATH/wiringPi-36fb7f1/target-rpi/lib/libwiringPi* 
sshpass -prpi scp -r $(MON_PATH)/target-rpi/bin/gpio pi@$(PI):/home/pi/objet_connecte

cc2 : Projet.c processus.c affichage.c conversion.c gestionFichier.c ./lib/libmcs.a
	echo $(DESTDIR)
	make ccLib
	$(CCC) -DSERVEUR -I$(DESTDIR)/include -L$(DESTDIR)/lib -L$(MON_PATH)/target_rpi/lib -I$(MON_PATH)/target_rpi/include -I$(MON_PATH)/target_rpi/include/ncurses -L./lib -std=gnu99 -o serveur_rpi affichage.c processus.c conversion.c gestionFichier.c Projet.c -lmcs -lpthread -lncurses
	$(CCC) -L./lib -DCROSS_COMPILE -L$(MON_PATH)/target_rpi/lib -I$(MON_PATH)/target_rpi/include -I$(MON_PATH)/target_rpi/include/ncurses -I$(DESTDIR)/include -L$(DESTDIR)/lib -std=gnu99 -o client_rpi processus.c affichage.c conversion.c gestionFichier.c Projet.c fonctionWiringPi.c -lmcs -lpthread -lwiringPi -lwiringPiDev -lncurses
	sshpass -prpi scp serveur_rpi  rpi@$(PI2):/home/rpi/objet_connecte
	sshpass -prpi scp client_rpi rpi@$(PI2):/home/rpi/objet_connecte
	sshpass -prpi scp -r ./dico rpi@$(PI2):/home/rpi/objet_connecte
	sshpass -prpi scp -r ./lib rpi@$(PI2):/home/rpi/objet_connecte
	sshpass -prpi scp -r $(MON_PATH)/target-rpi/bin/gpio pi@$(PI2):/home/pi/objet_connecte
#cp  $MON_PATH/wiringPi-36fb7f1/target-rpi/lib/libwiringPi*
	sshpass -prpi scp -r $(MON_PATH)/target-rpi/lib/libwiringPi* rpi@$(PI2):/home/rpi/objet_connecte

ccLib : 
	echo $(CCC)
	cd lib && make clean
	cd lib && make cc