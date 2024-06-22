
#include "include/fonctionWiringPi.h"

#define RELAY 40
#define BUZZER 12

#define PIN_BASE 100
#define I2C_ADDRESS_LCD 0x21
#define RS 1
#define E 2
#define DB4 3
#define DB5 4
#define DB6 5
#define DB7 6

int hexValues[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

int initLCD(){
    int i;
    int lcdHandle;
    int fd = wiringPiI2CSetup(I2C_ADDRESS_LCD);
    for (i=0; i<9; i++){
        wiringPiI2CWriteReg8(fd, i, 0xFF);
    }
    wiringPiSetupSys();
    mcp23008Setup(PIN_BASE, I2C_ADDRESS_LCD);
    pinMode(PIN_BASE, OUTPUT);

    lcdHandle = lcdInit(2, 16, 4, PIN_BASE+RS, PIN_BASE+E, PIN_BASE+DB4, PIN_BASE+DB5, PIN_BASE+DB6, PIN_BASE+DB7, 0, 0, 0, 0);
    if (lcdHandle == -1) {
        printf("lcdInit failed!\n");
        return -1;
    }
    lcdDisplay(lcdHandle, 1);
    return lcdHandle;
}


void writeLCD(int lcdHandle, int line, int column, const char* message){
    lcdPosition(lcdHandle, line, column);
    lcdPuts(lcdHandle, message);
}

void Relay()
{
    pinMode(RELAY, OUTPUT) ;
    digitalWrite (RELAY, HIGH) ;	// On
    delay (500) ;		// mS
    digitalWrite (RELAY, LOW) ;	// Off
    delay (500) ;
}
void Buzzer()
{   
    pinMode(BUZZER, OUTPUT);
    digitalWrite (BUZZER, HIGH) ;	// On
    delay (500) ;		// mS
    digitalWrite (BUZZER, LOW) ;	// Off
    delay (500) ;
}

void affichage7segments(int fd,int sec, int min){

    int unite_min= min % 10 ;
    int unite_sec= sec % 10;
    int dizaine_min= min / 10;
    int dizaine_sec=  sec / 10;

    wiringPiI2CWriteReg8(fd, 0x00, hexValues[dizaine_min]); // On affiche le chiffre 1 (tout à gauche)
    wiringPiI2CWriteReg8(fd, 0x02, hexValues[unite_min]); // On affiche le chiffre 2
    wiringPiI2CWriteReg8(fd, 0x04, 0x02);//On affiche ":"
    wiringPiI2CWriteReg8(fd, 0x06, hexValues[dizaine_sec]); // On affiche le chiffre 3
    wiringPiI2CWriteReg8(fd, 0x08, hexValues[unite_sec]); // On affiche le chiffre 4 (tout à droite)

}

void chrono(int fd){
    
    int sec=59; //car il y a un certains décalage entre le moment ou on lance le chrono et le moment ou il s'affiche
    int min=0; 
    affichage7segments(fd,sec,min);
    while (1){
    
    //printf("seconde=%d\n",sec);
    //printf("minute=%d\n",min);
    if(sec <= 10)
    {
        Relay();
    }
    else{
        delay(1000);
    }
    
    sec--; 
    affichage7segments(fd,sec,min);
    }
    
}

void JouerNoteDeFin()
{
    pinMode(BUZZER, OUTPUT);
    digitalWrite (BUZZER, HIGH) ;	// On
    delay (1000) ;		// mS
    digitalWrite (BUZZER, LOW) ;	// Off
    delay (1000) ;
}