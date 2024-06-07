/**
 *	\file		session.h
 *	\brief		Spécification de la couche Session
 *	\author		Léonore Legrand
 *	\date		25 mars 2023
 *	\version	1.0
 */
/*
*****************************************************************************************
 *	\noop		I N C L U D E S   S P E C I F I Q U E S
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "session.h"
#include <string.h>

/*
*****************************************************************************************
 *	\noop		D E F I N I T I O N   DES   M A C R O S
 */
/**
 *	\def		CHECK(sts, msg)
 *	\brief		Macro-fonction qui vérifie que sts est égal -1 (cas d'erreur : sts==-1) 
 *				En cas d'erreur, il y a affichage du message adéquat et fin d'exécution  
 */
#define CHECK(sts, msg) if ((sts)==-1) {perror(msg); exit(-1);}
/**
 *	\def		PAUSE(msg)
 *	\brief		Macro-fonction qui affiche msg et attend une entrée clavier  
 */
#define PAUSE(msg)	printf("%s [Appuyez sur entrée pour continuer]", msg); getchar();
/*
*****************************************************************************************

*****************************************************************************************
 *	\noop		P R O T O T Y P E S   DES   F O N C T I O N S
 */
/**
 *	\fn			void adr2struct (struct sockaddr_in *addr, char *adrIP, short port)
 *	\brief		Transformer une adresse au format humain en structure SocketBSD
 *	\param		addr : structure d'adressage BSD d'une socket INET
 *	\param		adrIP : adresse IP de la socket créée
 *	\param		port : port de la socket créée
 *	\note		Le domaine dépend du mode choisi (TCP/UDP)
 *	\result		paramètre *adr modifié
 */
void adr2struct (struct sockaddr_in *addr, char *adrIP, short port){
	
	//fprintf(stderr,"\tadrIP : %s\n",adrIP);
	//printf("\tport : %d\n",port);
	//printf("adr2struct\n");
	addr->sin_family = AF_INET;
    addr->sin_port = htons (port);
    addr->sin_addr.s_addr = inet_addr(adrIP);
	memset(&addr->sin_zero, 0, 8);
}

/**
 *	\fn			socket_t creerSocket (int mode)
 *	\brief		Création d'une socket de type DGRAM/STREAM
 *	\param		mode : mode connecté (STREAM) ou non (DGRAM)
 *	\result		socket créée selon le mode choisi
 */
socket_t creerSocket (int mode){
	socket_t sock;
	sock.mode = mode;
	CHECK(sock.fd = socket(AF_INET, mode, 0), "Can't create");
	return sock;
}

/**
 *	\fn			socket_t creerSocketAdr (int mode, char *adrIP, short port)
 *	\brief		Création d'une socket de type DGRAM/STREAM
 *	\param		mode : adresse IP de la socket créée
 *	\param		adrIP : adresse IP de la socket créée
 *	\param		port : port de la socket créée
 *	\result		socket créée dans le domaine choisi avec l'adressage fourni
 */
socket_t creerSocketAdr (int mode, char *adrIP, short port){
	//printf("creerSocketAddr\n");
	socket_t sock = creerSocket(mode);

	//printf("\tadrIP : %s\n",adrIP);
	//printf("\tport : %d\n",port);
	adr2struct (&sock.addrLoc, adrIP, port);
	//printf("after_adr2struct\n");
	CHECK(bind(sock.fd, (struct sockaddr *)&sock.addrLoc, sizeof sock.addrLoc) , "Can't bind");
	return sock;
}
/**
 *	\fn			creerSocketEcoute (char *adrIP, short port)
 *	\brief		Création d'une socket d'écoute avec l'adressage fourni en paramètre
 *	\param		adrIP : adresse IP du serveur à mettre en écoute
 *	\param		port : port TCP du serveur à mettre en écoute
 *	\result		socket créée avec l'adressage fourni en paramètre et dans un état d'écoute
 *	\note		Le domaine est nécessairement STREAM
 */
socket_t creerSocketEcoute (char *adrIP, short port){
	printf("Creer socket ecoute\n");
	socket_t sock = creerSocketAdr (SOCK_STREAM, adrIP, port);
	    // Mise en écoute de la socket
    CHECK(listen(sock.fd, 5) , "Can't calibrate");
	return sock;
}
/**
 *	\fn			socket_t accepterClt (const socket_t sockEcoute)
 *	\brief		Acceptation d'une demande de connexion d'un client
 *	\param		sockEcoute : socket d'écoute pour réception de la demande
 *	\result		socket (dialogue) connectée par le serveur avec un client
 */
socket_t accepterClt (const socket_t sockEcoute){
	socket_t sock;
	sock.mode= SOCK_STREAM;
	sock.addrLoc=sockEcoute.addrLoc;
	int cltLen=sizeof(sock.addrDst);
    CHECK(sock.fd=accept(sockEcoute.fd, (struct sockaddr *)&sock.addrDst, &cltLen), "Can't connect at accepterClt");
	return sock;
}
/**
 *	\fn			socket_t connecterClt2Srv (char *adrIP, short port)
 *	\brief		Création d'une socket d'appel et connexion au seveur dont
 *				l'adressage est fourni en paramètre
 *	\param		adrIP : adresse IP du serveur à connecter
 *	\param		port : port TCP du serveur à connecter
 *	\result		socket connectée au serveur fourni en paramètre
 */
socket_t connecterClt2Srv (char *adrIP, short port){
	socket_t sock_clt = creerSocket(SOCK_STREAM);

	adr2struct(&sock_clt.addrDst, adrIP, port);
	
	CHECK(connect(sock_clt.fd, (struct sockaddr *)&sock_clt.addrDst, sizeof sock_clt.addrDst) , "Can't connect at connecterClt2Srv");
	return sock_clt;
}

