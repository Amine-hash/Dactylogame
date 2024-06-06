/*
    Echange Client Serveur : 
    1 - Le serveur récupère les 100 mots aléatoires
    2 - Le serveur envoie les 100 mots au client
    3 - Le client affiche les 100 mots et lance un timer de 60 secondes - il ecoute la sortie standard pour récupérer les mots écrits par l'utilisateur
    4 - A la fin du timer, le client envoie le nombre de mots corrects au serveur
    5 - Le serveur affiche le nombre de mots corrects
*/

#include "include/Projet.h"
#include "include/affichage.h"
#include "include/gestionFichier.h"
#include "include/conversion.h"
#include "include/processus.h"
#include "lib/data.h"


buffer_t buff;
//argument de la fonction main
int main(int argc, char *argv[])
{
    char * ip_du_srv ;
    //on récupère l'IP du serveur en argument si aucun argument alors l'IP est la constante IP_SRV
    if (argc > 1)
    {
        ip_du_srv = argv[1];
    }
    else
    {
        ip_du_srv = IP_SRV;
    }
    #ifdef SERVEUR
        //atexit(bye());
        Serveur(ip_du_srv);
    #else
    
    Client(ip_du_srv);
    #endif
   
}

/**
 * @brief  Fonction client
 * 
 */
void Client(char * ip_srv) 
{        
    socket_t sock;
    buffer_t rep;
    buffer_t res ;

    int i;
    struct sigaction newact ;
    newact.sa_handler = stopTimer ;
    newact.sa_flags = 0 ;
    sigaction(SIGALRM, &newact, NULL) ;
    int pid ;
    int status;
    char list_100_mots[3000];
    char sequence[1024] = "";
    char tab_mot[100][30];
    char mot_ecrit[30];
    int CurseurMotEcrit = 0;
    int compteur_mot_correct = 0;
    int numeroDeLigne = 0;
    sock = connecterClt2Srv (ip_srv, PORT_SRV);
    PAUSE("client connecté");

    //on recoit les 100 mots
    recevoir(&sock, rep, NULL);
    strcat(sequence, rep);
    system("clear");
    get100MotsAleatoires(sequence,list_100_mots);
    ConversionTabMotsEnDoubleTableau(list_100_mots,tab_mot);
    pid=CreationFils();
    GestionFils(pid);
    while(CurseurMotEcrit < 100)
    {
        if(waitpid(pid, &status, WNOHANG) != 0)
        {
            break;
        }
        AffichageListeDeMots(tab_mot, CurseurMotEcrit);

        compteur_mot_correct = GestionPere(pid,tab_mot, CurseurMotEcrit, mot_ecrit, compteur_mot_correct);
        CurseurMotEcrit++;    
    }
    AffichageResultat(compteur_mot_correct);
    PAUSE("Envoyer resultats au serveur \n ");

    char resultat[10] ;
    sprintf(resultat, "%d", compteur_mot_correct);
    envoyer(&sock,resultat, NULL);
    //attente de la réponse du serveur
    recevoir(&sock, res, NULL);
    system("clear");
    printf("\n%s\n", res);
    //on attend une seconde avant de clore la connexion
    close(sock.fd);
}
/**
 * @brief  Fonction serveur
 * 
 */
void Serveur(char * ip_srv)
{
    printf("ip du serveur : %s\n", ip_srv);
    socket_t se;
    char sequence[1024] = "";
    char sequence_save[1024];
    int compteur_reponse = 0;
    int nbClientConnecte = 0;
    socket_t tab_sd[5];
    int tab_resulat[2];
    int ordreClient1 = 0;
    int ordreClient2 = 0;
    int choixMenu ;
    char *  choixMenuStr ;
    int choixDifficulte = 1 ;
    int choixNbJoueurs = 1 ;
    char * dictionnaire = "dico/dico_facile.txt";  

    while(choixMenu != 3)
        {
            CLEAR();
            printf("---Menu de choix du serveur---\n");
            printf("-1- Dificulté facile-intermédiaire-difficile\n");
            printf("-2- Nombre de joueurs\n");
            printf("-3- Lancer la partie\n");
            printf("-0- Quitter\n");
            printf("---------------------------------------------------------\n");
            printf("Paramètres actuels : Difficulté : %d - Nombre de joueurs : %d\n", choixDifficulte, choixNbJoueurs);
            printf("---------------------------------------------------------\n");
            //on récupère le choix du serveur uniquement des nombres
            scanf("%d", &choixMenu);
            if(choixMenu < 0 || choixMenu > 3)
            {
                printf("Choix invalide\n");
                choixMenu = 0;
            }
            switch (choixMenu)
            {
            case 1:
                printf("Selectionner la difficulté\n");
                //menu de choix de la difficulté
                printf("-1- Facile\n");
                printf("-2- Intermédiaire\n");
                printf("-3- Difficile\n");
                scanf("%d", &choixDifficulte);
                if(choixDifficulte < 1 || choixDifficulte > 3)
                {
                    printf("Choix invalide\n");
                    choixDifficulte = 1;
                }
                switch(choixDifficulte)
                {
                    case 1:
                        dictionnaire = "dico/dico_facile.txt";
                        break;
                    case 2:
                        dictionnaire = "dico/dico_intermediaire.txt";
                        break;
                    case 3:
                        dictionnaire = "dico/dico_difficile.txt";
                        break;
                }
                break;
            case 2:
                printf("Nombre de joueurs ( 5 maximum )\n");
                scanf("%d", &choixNbJoueurs);
                if ( choixNbJoueurs < 1 || choixNbJoueurs > 5)
                {
                    printf("Choix invalide\n");
                    choixNbJoueurs = 1;
                }
                break;
            case 3:
                printf("Lancer la partie\n");
                break;
            case 0:
                printf("Quitter\n");
                exit(0);
                break;
            }
        }

    int NbLignes = CompterLigneFichier(dictionnaire);
    // Création de la socket de réception des requêtes
    se = creerSocketEcoute (ip_srv, PORT_SRV);
    PAUSE("Socket crée");

    //Récupération des 100 mots aléatoires
    srand(time(0));
    get100NbAleatoires(sequence, NbLignes);
    strcat(sequence, dictionnaire);
    strcpy(sequence_save, sequence);
    // Boucle permanente de serveur
    while (1) {
        
        // Attente d’un appel
        while (nbClientConnecte < choixNbJoueurs)
        {
            socket_t sd = accepterClt(se);
            tab_sd[nbClientConnecte] = sd;
            nbClientConnecte++;
        }
        printf("Clients connectés\n");

        // Envoi des 100 mots aux clients
        for (int i = 0; i < choixNbJoueurs; i++)
        {
            printf("Envoi des 100 mots au client %d\n", i);
            envoyer(&tab_sd[i], sequence_save, NULL);
        }
        // Attente des réponses des clients (resultats)
        for (int i = 0; i < choixNbJoueurs; i++)
        {
            buffer_t rep;
            recevoir(&tab_sd[i], rep, NULL);
            tab_resulat[i] = atoi(rep);
        }
        char resultatClients[5][3];

        for (int i = 0; i < choixNbJoueurs ;i++)
        {
            convertirNbToCode(tab_resulat[i], resultatClients[i]);
        }
        
        char message[1024] = "";
        char message_save[1024] = "";
        char numeroClient[6];
        for (int i = 0; i < choixNbJoueurs; i++)
        {
            strcat(message, "Client ");
            convertirNbToCode(i, numeroClient);
            strcat(message, numeroClient );
            strcat(message, " : ");
            strcat(message, resultatClients[i]);
            strcat(message, " mots corrects\n");
        }

        //determination du gagnant
        int max = 0;
        int gagnant = 0;
        char gagnantStr[6];
        for (int i = 0 ; i < choixNbJoueurs; i++)
        {
            if (tab_resulat[i] > max)
            {
                max = tab_resulat[i];
                gagnant = i;
            }
        }
        strcat(message, "Gagnant : Client ");
        convertirNbToCode(gagnant, gagnantStr);
        strcat(message, gagnantStr);
        strcpy(message_save, message);

        // Envoi des résultats aux clients
        for (int i = 0; i < choixNbJoueurs; i++)
        {
            envoyer(&tab_sd[i], message, NULL);
        }  
    }
    close(se.fd);
}

void stopTimer(int signal_number)
{
     switch(signal_number)
    {
        case SIGALRM:
            //depassement de delai
            printf("\nTemps écoulé - Veuillez envoyer un message pour afficher les resultats\n");
            break;
    }
}
