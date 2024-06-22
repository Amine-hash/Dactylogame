#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <lcd.h>
#include <mcp23008.h>


#define I2C_ADDRESS 0x70
#define PIN_BASE 100
#define ADDRESS_MCP23008 0x20
#define I2C_ADDRESS_LCD 0x21 
 
#define PIN_BASE 100
#define I2C_ADDRESS_LCD 0x21
#define RS 1
#define E 2
#define DB4 3
#define DB5 4
#define DB6 5
#define DB7 6

void Delay();
void affichage7segments(int fd,int sec, int min);
void chrono(int fd);
void Buzzer();
void JouerNoteDeFin();
void pulseEnable();
int initLCD();
void writeLCD(int lcdHandle, int line, int column, const char* message);