#include "include/affichage.h"
#include <ncurses.h>
/**
 * @brief  Affichage du résultat final 
 * 
 * @param mot_correct 
 */
extern int lcdHandle;
void AffichageResultat(int mot_correct)
{
    printf("Nombre de mots corrects : %d\n", mot_correct);
    #ifdef CROSS_COMPILE
        wiringPiSetupPhys();
        lcdClear(lcdHandle);
        writeLCD(lcdHandle, 0, 0, "Score :");
        char str[50];
        sprintf(str, "%d mots corrects", mot_correct);
        writeLCD(lcdHandle, 0, 1, str);
    #endif
}


/**
 * @brief Affichage de la liste des mots
 * 
 * @param TabDoubleMot 
 * @param CurseurMotEcrit 
 */
void AffichageListeDeMots(char TabDoubleMot[100][30],int CurseurMotEcrit)
{   
    for (int i = CurseurMotEcrit; i < 100; i++)
            {
                if (i == CurseurMotEcrit)
                {
                    COUL(31);
                }
                else
                {
                    COUL(0);
                }
                for (int j = 0; j < strlen(TabDoubleMot[i]); j++)
                {
                    printf("%c", TabDoubleMot[i][j]);
                }
                printf(" ");
            }
            printf("\n");
}


param_partie_t Menu(param_partie_t parametres) {
    int choixMenu = 0;
    param_partie_t parametresRetour = parametres;
    int ch; // Variable pour stocker la touche pressée par l'utilisateur

    // Initialisation de ncurses
    initscr();
    cbreak();   // Désactive le buffering de ligne
    noecho();   // Désactive l'affichage automatique des caractères saisis
    keypad(stdscr, TRUE);   // Active la prise en charge des touches spéciales
    while (1) {
        clear();
        printw("-----------------Menu de choix du serveur-----------------\n");
        if (choixMenu == 1) {
            printw("-X- Difficulté facile-intermédiaire-difficile\n");
        } else {
            printw("-- Difficulté facile-intermédiaire-difficile\n");
        }
        
        if (choixMenu == 2) {
            printw("-X- Nombre de joueurs\n");
        } else {
            printw("-- Nombre de joueurs\n");
        }
        
        if (choixMenu == 3) {
            printw("-X- Lancer la partie\n");
        } else {
            printw("-- Lancer la partie\n");
        }
        
        if (choixMenu == 0) {
            printw("-X- Quitter\n");
        } else {
            printw("-- Quitter\n");

        }
        
        printw("---------------------------------------------------------\n");
        printw("Paramètres actuels : Difficulté : %d - Nombre de joueurs : %d\n", parametresRetour.choixDifficulte, parametresRetour.choixNbJoueurs);
        printw("---------------------------------------------------------\n");

        // Affichage de l'indicateur de sélection
        switch (choixMenu) {
            case 0:
                printw("Votre choix : Quitter\n");
                break;
            case 1:
                printw("Votre choix : Difficulté\n");
                break;
            case 2:
                printw("Votre choix : Nombre de joueurs\n");
                break;
            case 3:
                printw("Votre choix : Lancer la partie\n");
                break;
        }
        printw("Dernière touche appuyée : %d\n", ch);

        refresh();

        // Lecture de la touche pressée par l'utilisateur
        ch = getch();

        switch (ch) {
            case KEY_UP:
                choixMenu--;
                if (choixMenu < 0)
                    choixMenu = 3;
                break;
            case KEY_DOWN:
                choixMenu++;
                if (choixMenu > 3)
                    choixMenu = 0;
                break;
            case '\n': // Touche Entrée
                switch (choixMenu) {
                    case 0: // Quitter
                        clear();
                        printw("Quitter\n");
                        refresh();
                        endwin(); // Fin de ncurses
                        exit(0);
                        break;
                    case 1: // Difficulté
                        clear();
                        printw("Sélectionner la difficulté\n");
                        printw("- -1- Facile\n");
                        printw("- -2- Intermédiaire\n");
                        printw("- -3- Difficile\n");
                        refresh();
                        while (1) {
                            ch = getch();
                            if (ch == KEY_UP) {
                                parametresRetour.choixDifficulte--;
                                if (parametresRetour.choixDifficulte < 1)
                                    parametresRetour.choixDifficulte = 3;
                            } else if (ch == KEY_DOWN) {
                                parametresRetour.choixDifficulte++;
                                if (parametresRetour.choixDifficulte > 3)
                                    parametresRetour.choixDifficulte = 1;
                            } else if (ch == '\n') {
                                break;
                            }
                            clear();
                            printw("Sélectionner la difficulté\n");
                            printw("- -1- Facile\n");
                            printw("- -2- Intermédiaire\n");
                            printw("- -3- Difficile\n");
                            mvprintw(parametresRetour.choixDifficulte, 1, "X");
                            refresh();
                        }
                        break;
                    case 2: // Nombre de joueurs
                        clear();
                        printw("Nombre de joueurs ( 5 maximum )\n");
                        printw("- -1 joueur\n");
                        printw("- -2 joueurs\n");
                        printw("- -3 joueurs\n");
                        printw("- -4 joueurs\n");
                        printw("- -5 joueurs\n");
                        refresh();
                        while (1) {
                            ch = getch();
                            if (ch == KEY_UP) {
                                parametresRetour.choixNbJoueurs--;
                                if (parametresRetour.choixNbJoueurs < 1)
                                    parametresRetour.choixNbJoueurs = 5;
                            } else if (ch == KEY_DOWN) {
                                parametresRetour.choixNbJoueurs++;
                                if (parametresRetour.choixNbJoueurs > 5)
                                    parametresRetour.choixNbJoueurs = 1;
                            } else if (ch == '\n') {
                                break;
                            }
                            clear();
                        printw("Nombre de joueurs ( 5 maximum )\n");
                        printw("- -1 joueur\n");
                        printw("- -2 joueurs\n");
                        printw("- -3 joueurs\n");
                        printw("- -4 joueurs\n");
                        printw("- -5 joueurs\n");
                            mvprintw(parametresRetour.choixNbJoueurs, 1, "X");
                            refresh();
                        }
                        break;
                    case 3: // Lancer la partie
                        clear();
                        printw("Lancer la partie\n");
                        refresh();
                        endwin(); // Fin de ncurses
                        return parametresRetour;
                        break;
                }
                break;
        }
    }

    endwin(); // Fin de ncurses
    return parametresRetour;
}