
#include "include/fonctionWiringPi.h"

//fonction avec wiring Pi qui allume une led pendant 1 seconde puis l'éteinds
#define	LED	29
#define BUZZER 1

int hexValues[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
void Led()
{
    wiringPiSetup();
    pinMode(LED, OUTPUT) ;
    digitalWrite (LED, HIGH) ;	// On
    delay (500) ;		// mS
    digitalWrite (LED, LOW) ;	// Off
    delay (500) ;
}
void Buzzer()
{
    wiringPiSetup();
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
    
    int sec=0; //car il y a un certains décalage entre le moment ou on lance le chrono et le moment ou il s'affiche
    int min=0; 
    affichage7segments(fd,sec,min);
    while (1){
    
    //printf("seconde=%d\n",sec);
    //printf("minute=%d\n",min);
    if(sec >= 50)
    {
        Led();
    }
    else{
        delay(1000);
    }
    if (sec>=60){
      sec=0;
      min++;
      break;  
    }
    sec++; 
    affichage7segments(fd,sec,min);
    }
}

void JouerNoteDeFin()
{
    wiringPiSetup();
    pinMode(BUZZER, OUTPUT);
    digitalWrite (BUZZER, HIGH) ;	// On
    delay (1000) ;		// mS
    digitalWrite (BUZZER, LOW) ;	// Off
    delay (1000) ;
}