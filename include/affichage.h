/**
 * @file affichage.h
 * @author Théo COLIN - Léonore LEGRAND
 * @brief spécification des fonctions d'affichage
 * @version 0.1
 * @date 2024-05-04
 * 
 * 
 */

//si le fichier n'est pas défini
#ifndef AFFICHAGE_H
#include "../include/Projet.h"
#ifdef CROSS_COMPILE
    #include "../include/fonctionWiringPi.h"
#endif
#define AFFICHAGE_H
#endif
//prototypes
void AffichageResultat(int mot_correct);
void AffichageListeDeMots(char TabDoubleMot[100][30],int CurseurMotEcrit);
param_partie_t Menu(param_partie_t parametres);