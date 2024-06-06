/**
 * @file Projet.h
 * @author Théo COLIN - Léonore LEGRAND
 * @brief  spécification du fichier principal
 * @version 0.1
 * @date 2024-05-04
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <bits/sigaction.h>

//MACRO-CONSTANTES
/**
 * @brief  IP du serveur loopback 127.0.0.1 POUR TESTER EN LOCAL | POUR TESTER EN RESEAU METTRE L'IP DU SERVEUR
 * 
 */
#define IP_SRV  "127.0.0.1" //IP du serveur
/**
 * @brief  Port du serveur
 * 
 */
#define PORT_SRV 5000

//MACRO-FONCTIONS
/**
 * @brief  Permet de mettre en couleur le texte
 * 
 */
#define COUL(coul) printf("\e[%dm", coul);

/**
 * @brief  Efface l'ecran avec la commande clear
 * 
 */
#define CLEAR() system("clear");  


//prototypes

void Client(char * ip_srv);
void Serveur(char * ip_srv);
void stopTimer(int signal_number);
