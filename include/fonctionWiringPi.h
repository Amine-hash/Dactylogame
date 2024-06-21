#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>


#define I2C_ADDRESS 0x70
#define PIN_BASE 100
#define ADDRESS_MCP23008 0x20
#define I2C_ADDRESS_LCD 0x21 

#define RS 1
#define E 2
#define DB4 3
#define DB5 4
#define DB6 5
#define DB7 6 

void Led();
void affichage7segments(int fd,int sec, int min);
void chrono(int fd);
void Buzzer();
void JouerNoteDeFin();
void pulseEnable();
void lcd_byte(char bits);
void SetCmdMode();
void SetChrMode();
void lcd_text(char *s);
void lcd_init(); 