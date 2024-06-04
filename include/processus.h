/**
 * @file processus.h
 * @author Théo COLIN - Léonore LEGRAND
 * @brief  spécification des fonctions de gestion des processus
 * @version 0.1
 * @date 2024-05-04
 * 
 * 
 */

#include "../lib/session.h"
#include "../include/Projet.h"

//prototypes
void stopTimer(int signal_number);
int CreationFils();
void GestionFils(int pid);
int GestionPere(int pid , char DoubleTab[100][30] , int compteur_mot_ecrit, char mot_ecrit[30], int mot_correct);