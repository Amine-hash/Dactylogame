#include "include/gestionFichier.h"

/**
 * @brief  Compte le nombre de ligne d'un fichier
 * 
 * @param nomFichier 
 * @return long 
 */
long CompterLigneFichier(char *nomFichier)
{
    FILE *fp;
    fp = fopen(nomFichier, "r");
    if (fp == NULL)
    {
        printf("Probleme ouverture de fichier lorsqu'on compte le nombre de ligne\n");
        exit(1);
    }
    long compteur = 0;
    char c;
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n')
        {
            compteur++;
        }
    }
    fclose(fp);
    return compteur;
}


/**
 * @brief Récupère 100 mots aléatoires dans un dictionnaire français
 * @param char liste_mots[3000] - liste de 100 mots séparés par un espace
 *
 * @return void
 */
void get100MotsAleatoires(char sequence[1024], char liste_mots[3000])
{
    //initialisation du générateur de nombres aléatoires
    //le fichier possède 336 531 mots - on en extrait 100
    //On souhaite stocker les 100 mots dans une chaine de caractères, chaque mot étant séparé par un espace 
    long nummot ;
    long compteur = 0;
    int m = 0;
    long tabNumLigne[100];
    long numeroDeLigne = 0;
    int curseurTab = 0;
    char * dictionnaire ; 

    //Décodage des 100 codes avec le separateur "-"
    char *token = strtok(sequence, "-");

    //On récupère le nom du dictionnaire qui se situe après les 100 codes
    while (token != NULL)
    {
        numeroDeLigne = convertirCodeToNb(token);
        token = strtok(NULL, "-");
        tabNumLigne[curseurTab] = numeroDeLigne;
        curseurTab++;
        //on récupère le nom du dictionnaire après le dernier code 
        if (curseurTab == 100)
        {
            dictionnaire = token;
        }    
    }
    //printf("Dictionnaire : %s\n", dictionnaire);
    for (int i = 0; i < 100; i++)
    {
        nummot = tabNumLigne[i];
        //on parcourt le fichier jusqu'au "\n"du numero du mot
        FILE *fp;
        
        fp = fopen(dictionnaire, "r");
        if (fp == NULL)
        {
            printf("Impossible d'ouvrir le fichier\n");
            exit(1);
        }
        while(compteur < nummot)
        {
            //ouverture fichier
            if (fgetc(fp) == '\n')
            {
                compteur++;
            }
        }
        //on est arrivé au bon mot
        while(fgetc(fp) != '\n')
        {
            fseek(fp, -1, SEEK_CUR);
            char c = fgetc(fp);
            //printf("%c",c);
            liste_mots[m] = c;
            m++;
        }
        //printf(" ");
        liste_mots[m] = ' ';
        m++;
        fclose(fp);
        compteur = 0;     
    }
    liste_mots[m] = '\0';
    return ;
}

/**
 * @brief  Récupère 100 nombres aléatoires
 * 
 * @param sequence 
 * @param NbLignes 
 */
void get100NbAleatoires(char sequence[1024],long NbLignes)
{
    char code[6]; // Ajout d'un caractère pour le caractère nul '\0'
    long nunmot = 0;
    long numeroDeLigne = 0;
    
    for (int i = 0; i < 100; i++)
    {
        nunmot = rand() % NbLignes;
        convertirNbToCode(nunmot, code);
        strcat(sequence, code); // Concaténer code à sequence
        strcat(sequence, "-");  // Ajouter un séparateur "-" à la fin de sequence
    }
}