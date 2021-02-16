#include "s4d_breadboard.h"

/*
  Fur Elise - Ludwig van Beethovem
  Score available at https://musescore.com/user/28149610/scores/5281944
  Code based on the version at https://github.com/robsoncouto/arduino-songs
*/

const int NOTE_B0  =   31;
const int NOTE_C1  =   33;
const int NOTE_CS1 =   35;
const int NOTE_D1  =   37;
const int NOTE_DS1 =   39;
const int NOTE_E1  =   41;
const int NOTE_F1  =   44;
const int NOTE_FS1 =   46;
const int NOTE_G1  =   49;
const int NOTE_GS1 =   52;
const int NOTE_A1  =   55;
const int NOTE_AS1 =   58;
const int NOTE_B1  =   62;
const int NOTE_C2  =   65;
const int NOTE_CS2 =   69;
const int NOTE_D2  =   73;
const int NOTE_DS2 =   78;
const int NOTE_E2  =   82;
const int NOTE_F2  =   87;
const int NOTE_FS2 =   93;
const int NOTE_G2  =   98;
const int NOTE_GS2 =  104;
const int NOTE_A2  =  110;
const int NOTE_AS2 =  117;
const int NOTE_B2  =  123;
const int NOTE_C3  =  131;
const int NOTE_CS3 =  139;
const int NOTE_D3  =  147;
const int NOTE_DS3 =  156;
const int NOTE_E3  =  165;
const int NOTE_F3  =  175;
const int NOTE_FS3 =  185;
const int NOTE_G3  =  196;
const int NOTE_GS3 =  208;
const int NOTE_A3  =  220;
const int NOTE_AS3 =  233;
const int NOTE_B3  =  247;
const int NOTE_C4  =  262;
const int NOTE_CS4 =  277;
const int NOTE_D4  =  294;
const int NOTE_DS4 =  311;
const int NOTE_E4  =  330;
const int NOTE_F4  =  349;
const int NOTE_FS4 =  370;
const int NOTE_G4  =  392;
const int NOTE_GS4 =  415;
const int NOTE_A4  =  440;
const int NOTE_AS4 =  466;
const int NOTE_B4  =  494;
const int NOTE_C5  =  523;
const int NOTE_CS5 =  554;
const int NOTE_D5  =  587;
const int NOTE_DS5 =  622;
const int NOTE_E5  =  659;
const int NOTE_F5  =  698;
const int NOTE_FS5 =  740;
const int NOTE_G5  =  784;
const int NOTE_GS5 =  831;
const int NOTE_A5  =  880;
const int NOTE_AS5 =  932;
const int NOTE_B5  =  988;
const int NOTE_C6  = 1047;
const int NOTE_CS6 = 1109;
const int NOTE_D6  = 1175;
const int NOTE_DS6 = 1245;
const int NOTE_E6  = 1319;
const int NOTE_F6  = 1397;
const int NOTE_FS6 = 1480;
const int NOTE_G6  = 1568;
const int NOTE_GS6 = 1661;
const int NOTE_A6  = 1760;
const int NOTE_AS6 = 1865;
const int NOTE_B6  = 1976;
const int NOTE_C7  = 2093;
const int NOTE_CS7 = 2217;
const int NOTE_D7  = 2349;
const int NOTE_DS7 = 2489;
const int NOTE_E7  = 2637;
const int NOTE_F7  = 2794;
const int NOTE_FS7 = 2960;
const int NOTE_G7  = 3136;
const int NOTE_GS7 = 3322;
const int NOTE_A7  = 3520;
const int NOTE_AS7 = 3729;
const int NOTE_B7  = 3951;
const int NOTE_C8  = 4186;
const int NOTE_CS8 = 4435;
const int NOTE_D8  = 4699;
const int NOTE_DS8 = 4978;
const int REST     =    0;


int melodyDurations[] = {
  188, 188, 188, 188, 188, 
  188, 188, 188, 563, 20, 
  188, 188, 188, 563, 20, 
  188, 188, 188, 375, 188, 
  
  188, 188, 188, 188, 188, 
  188, 188, 188, 188, 563, 20, 
  188, 188, 188, 563, 20, 
  188, 188, 188, 750, 375, 
  
  188, 188, 188, 563, 20,
  188, 188, 188, 563, 20,
  188, 188, 188, 563, 20,
  188, 188, 188, 375, 188,
  
  188, 188, 188, 188, 188, 
  188, 188, 188, 188, 563, 20,
  188, 188, 188, 563, 20,
  188, 188, 188, 750, 30000, 
};


int melodyFrequencies[] = {
  
  NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_E5,  
  NOTE_B4, NOTE_D5,  NOTE_C5, NOTE_A4,  REST, 
  NOTE_C4, NOTE_E4,  NOTE_A4, NOTE_B4,  REST, 
  NOTE_E4, NOTE_GS4, NOTE_B4, NOTE_C5,  REST, 
  
  NOTE_E4,  NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, 
  NOTE_E5,  NOTE_B4, NOTE_D5,  NOTE_C5, NOTE_A4,  REST,  
  NOTE_C4,  NOTE_E4, NOTE_A4,  NOTE_B4, REST,    
  NOTE_E4,  NOTE_C5, NOTE_B4,  NOTE_A4, REST,

  NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, REST,
  NOTE_G4, NOTE_F5, NOTE_E5, NOTE_D5, REST,
  NOTE_F4, NOTE_E5, NOTE_D5, NOTE_C5, REST,
  NOTE_E4, NOTE_D5, NOTE_C5, NOTE_B4, REST,

  NOTE_E4, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, 
  NOTE_E5, NOTE_B4, NOTE_D5, NOTE_C5, NOTE_A4,  REST,
  NOTE_C4, NOTE_E4, NOTE_A4, NOTE_B4, REST,
  NOTE_E4, NOTE_C5, NOTE_B4, NOTE_A4, REST
};

int ledPins[] = { LED_BLUE, LED_GREEN, LED_YELLOW, LED_RED };

void setup() {  
  initializeBreadboard();
}

int loopCounter = 0;

void loop() {
  int ledIndex  = loopCounter % 4;
  int ledPin    = ledPins[ledIndex];

  int noteIndex = loopCounter % 82;
  int noteDuration = melodyDurations[noteIndex];
  int noteFrequency = melodyFrequencies[noteIndex];

  digitalWrite( ledPin, HIGH );
  playTone( noteFrequency, noteDuration );
  digitalWrite( ledPin, LOW );
  loopCounter++;
}
