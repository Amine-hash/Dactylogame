/**
 * @file conversion.h
 * @author Théo COLIN - Léonore LEGRAND
 * @brief  spécification des fonctions de conversion
 * @version 0.1
 * @date 2024-05-04
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//prototypes

void convertirNbToCode(long nb, char nbCodee[6]);
long convertirCodeToNb(char code[6]);
void ConversionTabMotsEnDoubleTableau(char listeMots[3000], char tab_mot[100][30]);
