#include "include/processus.h"
#ifdef CROSS_COMPILE
    #include <wiringPi.h>
    #include "include/fonctionWiringPi.h"
#endif

/**
 * @brief Création d'un processus fils
 * 
 * @return int pid 
 */
int CreationFils()
{
    int pid;
    int status;
    CHECK(pid = fork(),"Ne peut pas créer un nouveau processus");
    return pid;
}

/**
 * @brief Le fils lance un timer de 60 secondes
 * 
 * @param pid 
 */
void GestionFils(int pid)
{
    //le fils le fils compte jusqu'à 60 secondes et s'arrête
    if(pid == 0)
    {
        //on lance le timer
        alarm(60);
        //creation d'un thread qui compte chaque seconde
        pthread_t thread_compteur;
        pthread_create(&thread_compteur, NULL,TraitementCompteur, NULL);
        pause();
        #ifdef CROSS_COMPILE
            JouerNoteDeFin();
        #endif
        exit(0);
    }
}

/**
 * @brief ecoute des mots écrits par l'utilisateur et vérification de la validité du mot si oui incrémentation du compteur de mot correct
 * 
 * @param pid 
 * @param DoubleTab 
 * @param compteur_mot_ecrit 
 * @param mot_ecrit 
 * @param mot_correct 
 * @return int  mot_correct
 */
int GestionPere(int pid , char DoubleTab[100][30] , int compteur_mot_ecrit , char mot_ecrit[30] , int mot_correct)
{
    scanf("%s", mot_ecrit);
    //clear de la console
    CLEAR();
    compteur_mot_ecrit++;

    //on ajoute \0 à la fin du mot
    mot_ecrit[strlen(mot_ecrit)] = '\0';
    //vérification du mot
    
    if(strcmp(mot_ecrit, DoubleTab[compteur_mot_ecrit-1])==0)
    {
        mot_correct++;
    }
    return mot_correct;
}

/**
 * @brief Fonction qui compte le temps
 * 
 * @param arg 
 * @return void* 
 */
void *TraitementCompteur(void *arg)
{
    int i = 0;
    while (i < 60)
    {
        #ifdef CROSS_COMPILE
            int fd = wiringPiI2CSetup(I2C_ADDRESS); // Initialisation de l'afficheur 7 segments
            // Configuration de l'afficheur 7 segments
            wiringPiI2CWriteReg8(fd, 0x21, 0x01);
            wiringPiI2CWriteReg8(fd, 0x81, 0x00); 
            wiringPiI2CWriteReg8(fd, 0xef, 0x00);
            chrono(fd);
        #else
        sleep(1);
        #endif
        i++;
    }
    //fermeture du thread
    pthread_exit(NULL);
    return NULL;
}
