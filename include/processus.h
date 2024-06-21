/**
 * @file processus.h
 * @author Théo COLIN - Léonore LEGRAND
 * @brief  spécification des fonctions de gestion des processus
 * @version 0.1
 * @date 2024-05-04
 * 
 * 
 */
//si le fichier n'est pas défini
#ifndef PROCESSUS_H
#include "../lib/session.h"
#include "../include/Projet.h"
#include <pthread.h>


#define PROCESSUS_H
#endif
//prototypes
void stopTimer(int signal_number);
#include <stdio.h>
int CreationFils();
void GestionFils(int pid);
int GestionPere(int pid , char DoubleTab[100][30] , int compteur_mot_ecrit, char mot_ecrit[30], int mot_correct);
void *TraitementCompteur(void *arg);
void Led();