#include "include/affichage.h"

/**
 * @brief  Affichage du résultat final 
 * 
 * @param mot_correct 
 */
void AffichageResultat(int mot_correct)
{
    printf("Nombre de mots corrects : %d\n", mot_correct);
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

