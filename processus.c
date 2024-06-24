#include "include/processus.h"
#ifdef CROSS_COMPILE
    #include <wiringPi.h>
    #include "include/fonctionWiringPi.h"
    extern int lcdHandle;
#endif
extern volatile sig_atomic_t end;

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
            wiringPiSetupPhys();
            JouerNoteDeFin();
        #endif
        //envoie du signal SIGINT au père
        kill(getppid(), SIGUSR1);
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
    char buffer[30];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        if (end == 1) {
            printf("Fin du jeu.....\n");
            return mot_correct;
        } 
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(mot_ecrit, buffer);
    //clear de la console
    CLEAR();
    compteur_mot_ecrit++;

    //on ajoute \0 à la fin du mot
    mot_ecrit[strlen(mot_ecrit)] = '\0';
    //vérification du mot
    
    if(strcmp(mot_ecrit, DoubleTab[compteur_mot_ecrit-1])==0)
    {
        #ifdef CROSS_COMPILE
        wiringPiSetupSys();
        lcdClear(lcdHandle);
        writeLCD(lcdHandle, 0, 0, "Vrai");
        #endif

        mot_correct++;
    }
    else
    {
        #ifdef CROSS_COMPILE
        wiringPiSetupPhys();
        lcdClear(lcdHandle);
        writeLCD(lcdHandle, 0, 0, "Faux");
        #endif
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
    int i = 60;
    

    while (i >= 0)
    {
        #ifdef CROSS_COMPILE
            wiringPiSetupPhys();
            int fd = wiringPiI2CSetup(I2C_ADDRESS); // Initialisation de l'afficheur 7 segments
            // Configuration de l'afficheur 7 segments
            wiringPiI2CWriteReg8(fd, 0x21, 0x01);
            wiringPiI2CWriteReg8(fd, 0x81, 0x00); 
            wiringPiI2CWriteReg8(fd, 0xef, 0x00);
            chrono(fd);
        #else
        sleep(1);
        #endif
        i--;
    }
    //fermeture du thread
    pthread_exit(NULL);
    return NULL;
}
