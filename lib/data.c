/**
 *	\file		data.h
 *	\brief		Spécification de la couche Data Representation
 *	\author		Léonore Legrand
 *	\date		25 mars 2023
 *	\version	1.0
 */

/*
*****************************************************************************************
 *	\noop		I N C L U D E S   S P E C I F I Q U E S
 */
#include "session.h"
#include "data.h"
#include <stdarg.h>

/*
*****************************************************************************************
 *	\noop		D E F I N I T I O N   DES   C O N S T A N T E S
 */
/**
 *	\def		MAX_BUFFER
 *	\brief		taille d'un buffer_t d'émission/réception
 */
#define MAX_BUFFER	1024
/*
*****************************************************************************************
 *	\noop		S T R C T U R E S   DE   D O N N E E S
 */
/**
 *	\typedef	buffer_t
 *	\brief		chaîne de caractères à émettre/recevoir
 */
typedef char buffer_t[MAX_BUFFER];
/**
 *	\typedef	generic
 *	\brief		type de données générique : requêtes/réponses
 */
typedef void * generic;
/**
 *	\typedef	pFct
 *	\brief		pointer sur fonction générique à 2 parametres génériques
 */
typedef void (*pFct) (generic, generic);
/*
*****************************************************************************************
 *	\noop		P R O T O T Y P E S   DES   F O N C T I O N S
 */

void envoyerMessDGRAM(socket_t *sockEch, buffer_t buff, char *adrIP, int port){
    adr2struct (&sockEch->addrDst, adrIP, port);
    CHECK(sendto(sockEch->fd, buff, strlen(buff)+1, 0,(struct sockaddr *)&sockEch->addrDst, sizeof sockEch->addrDst) , "Can't send MESSDGRAM");
}

void envoyerMessSTREAM(socket_t *sockEch, buffer_t buff){
    //printf("je suis dans la fonction envoyerMessSTREAM");
    CHECK(send(sockEch->fd, buff, strlen(buff)+1, 0), "Can't send message stream");
}

/**
 *	\fn			void envoyer(socket_t *sockEch, generic quoi, pFct serial, ...)
 *	\brief		Envoi d'une requête/réponse sur une socket
 *	\param 		sockEch : socket d'échange à utiliser pour l'envoi
 *	\param 		quoi : requête/réponse à serialiser avant l'envoi
 *	\param 		serial : pointeur sur la fonction de serialisation d'une requête/réponse
 *	\note		si le paramètre serial vaut NULL alors quoi est une chaîne de caractères
 *	\note		Si le mode est DGRAM, l'appel nécessite en plus l'adresse IP et le port.
 *	\result		paramètre sockEch modifié pour le mode DGRAM
 */
/*void envoyer(socket_t *sockEch, generic quoi, pFct serial, ...){
    //envoyer(socket_t *sockEch, generic quoi, pFct serial,char * adrIP, int port)
    buffer_t buff; //buffer d'envoi

    //sérialiser dans buff la req/rep à envoyer
    if (serial != NULL) serial(quoi, buff);
    else strcpy(buff, (char *)quoi);

    //envoi: appel de la fonction adéquate selon le mode
    if (sockEch->mode==SOCK_STREAM) 
        {envoyerMessSTREAM(sockEch,buff);
        }
    else{
        va_list pArg; //liste des parametres 
        va_start(pArg, serial);
        char *adrIP = va_arg(pArg, char*);
        int port = va_arg(pArg,int);
        envoyerMessDGRAM(sockEch,buff, adrIP, port);
        va_end(pArg);
    }

}*/
void envoyer(socket_t *sockEch, generic quoi,pFct serial, ...){
    buffer_t buff; //buffer d'envoie

    //serialiser les data dans buff
    if(serial !=NULL ) serial(quoi,buff);
    else strcpy(buff,(char*)quoi);

    if (sockEch->mode == SOCK_DGRAM){
        va_list pArg;
        va_start(pArg, serial);
            char *adrIp=va_arg(pArg, char *);
            int port = va_arg(pArg, int);
            //printf("modedgram[%s]\n", buff);
            envoyerMessDGRAM(sockEch, buff, adrIp, port);
        va_end(pArg);
    }
    else if(sockEch->mode == SOCK_STREAM){
        //printf("modestream [%s]\n", buff);
        envoyerMessSTREAM(sockEch,buff);
    }
}


void recevoirMessDGRAM(socket_t *sockEch, buffer_t buff){
    socklen_t lenAdr = sizeof sockEch;
    CHECK(recvfrom(sockEch->fd, buff, sizeof(buffer_t), 0,(struct sockaddr *)&sockEch->addrDst, &lenAdr) , "Can't receive");
}

void recevoirMessSTREAM(socket_t *sockEch, buffer_t buff){
    CHECK(recv(sockEch->fd, buff, sizeof(buffer_t), 0) , "Can't receive en stream");
}

/*
 *	\fn			void recevoir(socket_t *sockEch, generic quoi, pFct deSerial)
 *	\brief		Réception d'une requête/réponse sur une socket
 *	\param 		sockEch : socket d'échange à utiliser pour la réception
 *	\param 		quoi : requête/réponse reçue après dé-serialisation du buffer de réception
 *	\param 		deSerial : pointeur sur la fonction de dé-serialisation d'une requête/réponse
 *	\note		si le paramètre deSerial vaut NULL alors quoi est une chaîne de caractères
 *	\result		paramètre quoi modifié avec le requête/réponse reçue
 *				paramètre sockEch modifié pour le mode DGRAM
 */
void recevoir(socket_t *sockEch, generic quoi, pFct deSerial){
    buffer_t buff; //buffer d'envoi

    //recevoir
    if (sockEch->mode==SOCK_STREAM) recevoirMessSTREAM(sockEch,buff);
    else recevoirMessDGRAM(sockEch,buff);

    //desérialiser dans buff la req/rep à envoyer
    if (deSerial != NULL) deSerial(buff, quoi);
    else strcpy(quoi,(generic)buff);   
}
