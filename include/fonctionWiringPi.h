#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define I2C_ADDRESS 0x70

void Led();
void affichage7segments(int fd,int sec, int min);
void chrono(int fd);
void Buzzer();