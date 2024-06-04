#include "include/conversion.h"
/**
 * @brief  Conversion d'un nombre en chaine de caractère
 * 
 * @param nb 
 * @param nbCodee 
 */
void convertirNbToCode(long nb, char nbCodee[6])
{
    //Objectif convertit le nombre en chaine de caractère
    sprintf(nbCodee, "%ld", nb);
}

/**
 * @brief  Conversion d'un code en nombre
 * 
 * @param code 
 * @return long 
 */
long convertirCodeToNb(char code[6])
{
    //Objectif convertir un char de 6 nombres maximum en long
    char *ptr;
    long nb = 0;
    nb = strtol(code, &ptr, 10);
    return nb ;
}

/**
 * @brief  Conversion de la liste de mots en double tableau du type tab[position du mot][lettre du mot]
 * 
 * @param listeMots 
 * @param tab_mot 
 */
void ConversionTabMotsEnDoubleTableau(char listeMots[3000], char tab_mot[100][30])
{
    //on stocke les 100 mots dans un double tableau du type tab[position du mot][lettre du mot]
    //on initialise le tableau de mots
    int k = 0;
    int l = 0;
    int compteur_mot_ecrit = 0;

        for (int i = 0; i < strlen(listeMots); i++)
        {        
            if (listeMots[i] == ' ')
            {
                tab_mot[k][l] = '\0';
                k++;
                l = 0;
            }
            else
            {
                tab_mot[k][l] = listeMots[i];
                l++;
            }
        }
}

