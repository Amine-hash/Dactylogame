#include "include/Projet.h"
#include "include/affichage.h"
#include "include/gestionFichier.h"
#include "include/conversion.h"
#include "include/processus.h"
#include "lib/data.h"

#ifdef CROSS_COMPILE
    #include <wiringPi.h>
    #include "include/fonctionWiringPi.h"
    int lcdHandle;
#endif

typedef struct user {
    char nom[30];
    int score;
    int position;
} user_t;

volatile sig_atomic_t end = 0;

buffer_t buff;

// Argument de la fonction main
int main(int argc, char *argv[]) {
    char *ip_du_srv;
    if (argc > 1) {
        ip_du_srv = argv[1];
    } else {
        ip_du_srv = IP_SRV;
    }

    #ifdef SERVEUR
        Serveur(ip_du_srv);
    #else
        Client(ip_du_srv);
    #endif
}

// Fonction client
void Client(char *ip_srv) {
    socket_t sock;
    buffer_t rep;
    buffer_t res;

    #ifdef CROSS_COMPILE
       wiringPiSetupPhys();
       lcdHandle = initLCD();
    #endif

    struct sigaction newact;
    newact.sa_handler = stopTimer;
    newact.sa_flags = 0;
    sigaction(SIGALRM, &newact, NULL);
    sigaction(SIGUSR1, &newact, NULL);

    int pid;
    int status;
    char list_100_mots[3000];
    char sequence[1024] = "";
    char tab_mot[100][30];
    char mot_ecrit[30];
    int CurseurMotEcrit = 0;
    int compteur_mot_correct = 0;

    sock = connecterClt2Srv(ip_srv, PORT_SRV);
    printf("En attente de connexion de l'ensemble des clients...\n");

    printf("Entrez votre nom d'utilisateur : ");
    scanf("%s", buff);
    envoyer(&sock, buff, NULL);

    recevoir(&sock, rep, NULL);
    strcat(sequence, rep);
    system("clear");
    get100MotsAleatoires(sequence, list_100_mots);
    ConversionTabMotsEnDoubleTableau(list_100_mots, tab_mot);

    for (int i = 3; i > 0; i--) {
        #ifdef CROSS_COMPILE
            printf("Début de la partie dans %d secondes\n", i);
            sleep(1);
            wiringPiSetupPhys();
            Buzzer();
        #else
            printf("Début de la partie dans %d secondes\n", i);
            sleep(1);
        #endif
        system("clear");
    }

    pid = CreationFils();
    GestionFils(pid);

    while (CurseurMotEcrit < 100) {
        if (waitpid(pid, &status, WNOHANG) != 0) {
            break;
        }

        AffichageListeDeMots(tab_mot, CurseurMotEcrit);
        compteur_mot_correct = GestionPere(pid, tab_mot, CurseurMotEcrit, mot_ecrit, compteur_mot_correct);

        if (end == 1) {
            break;
        }

        CurseurMotEcrit++;
    }

    AffichageResultat(compteur_mot_correct);
    PAUSE("Envoyer resultats au serveur \n");

    char resultat[10];
    sprintf(resultat, "%d", compteur_mot_correct);
    envoyer(&sock, resultat, NULL);

    printf("Attente de la réponse du serveur\n");
    recevoir(&sock, res, NULL);
    system("clear");
    printf("\n%s\n", res);
    close(sock.fd);
}

// Fonction serveur
void Serveur(char *ip_srv) {
    printf("ip du serveur : %s\n", ip_srv);
    socket_t se;
    char sequence[1024] = "";
    char sequence_save[1024];
    int nbClientConnecte = 0;
    socket_t tab_sd[5];
    int tab_resultat[5];
    user_t tab_user[5];
    param_partie_t parametres;
    parametres.choixDifficulte = 1;
    parametres.choixNbJoueurs = 1;
    parametres.dictionnaire = "dico/dico_facile.txt";

    parametres = Menu(parametres);
    int NbLignes = CompterLigneFichier(parametres.dictionnaire);
    se = creerSocketEcoute(ip_srv, PORT_SRV);

    srand(time(0));
    get100NbAleatoires(sequence, NbLignes);
    strcat(sequence, parametres.dictionnaire);
    strcpy(sequence_save, sequence);

    while (1) {
        while (nbClientConnecte < parametres.choixNbJoueurs) {
            socket_t sd = accepterClt(se);
            tab_sd[nbClientConnecte] = sd;
            nbClientConnecte++;
            tab_user[nbClientConnecte].score = 0;
            tab_user[nbClientConnecte].position = nbClientConnecte;
        }

        printf("Clients connectés\n");
        int compteur_nom_user = 0;

        while (compteur_nom_user < parametres.choixNbJoueurs) {
            buffer_t rep;
            memset(rep, 0, sizeof(rep));
            recevoir(&tab_sd[compteur_nom_user], rep, NULL);
            strcpy(tab_user[compteur_nom_user].nom, rep);
            compteur_nom_user++;
        }

        for (int i = 0; i < parametres.choixNbJoueurs; i++) {
            envoyer(&tab_sd[i], sequence_save, NULL);
        }

        for (int i = 0; i < parametres.choixNbJoueurs; i++) {
            buffer_t rep;
            memset(rep, 0, sizeof(rep));
            recevoir(&tab_sd[i], rep, NULL);
            tab_resultat[i] = atoi(rep);
            tab_user[i].score = tab_resultat[i];
        }

        printf("Résultats reçus\n");
        char resultatClients[5][3];
        for (int i = 0; i < parametres.choixNbJoueurs; i++) {
            convertirNbToCode(tab_resultat[i], resultatClients[i]);
            tab_user[i].score = tab_resultat[i];
        }

        char message[1024] = "";
        for (int i = 0; i < parametres.choixNbJoueurs; i++) {
            strcat(message, "Client ");
            char numeroClient[6];
            convertirNbToCode(i, numeroClient);
            strcat(message, numeroClient);
            strcat(message, " - ");
            strcat(message, tab_user[i].nom);
            strcat(message, " : ");
            strcat(message, resultatClients[i]);
            strcat(message, " mots corrects\n");
        }

        int max = 0;
        int gagnant = 0;
        char *pseudo_gagnant;
        for (int i = 0; i < parametres.choixNbJoueurs; i++) {
            if (tab_resultat[i] > max) {
                max = tab_resultat[i];
                gagnant = i;
                pseudo_gagnant = tab_user[i].nom;
            }
        }

        strcat(message, "Gagnant : Client ");
        char gagnantStr[6];
        convertirNbToCode(gagnant, gagnantStr);
        strcat(message, gagnantStr);
        strcat(message, " ");
        strcat(message, pseudo_gagnant);

        for (int i = 0; i < parametres.choixNbJoueurs; i++) {
            if (tab_resultat[i] == max && i != gagnant) {
                strcat(message, " égalité avec le Client ");
                char iStr[6];
                convertirNbToCode(i, iStr);
                strcat(message, iStr);
                strcat(message, " ");
                strcat(message, tab_user[i].nom);
            }
        }

        for (int i = 0; i < parametres.choixNbJoueurs; i++) {
            if (tab_sd[i].fd != -1) {
                envoyer(&tab_sd[i], message, NULL);
            }
        }

        nbClientConnecte = 0;
        close(se.fd);
        sleep(1);
        exit(0);
    }
}

void stopTimer(int signal_number) {
    switch(signal_number) {
        case SIGALRM:
            printf("\nTemps écoulé - Veuillez envoyer un message pour afficher les resultats\n");
            break;
        case SIGUSR1:
            end = 1;
            break;
    }
}

