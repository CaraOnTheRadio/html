#ifndef S4D_BREADBOARD_H
#define S4D_BREADBOARD_H

#include <Arduino.h>

#ifndef NO_OLED         
#include <U8g2lib.h>
void initializeOLED();
#endif
void playTone( int, int );

/* * * * * * * * * * * * * * * * * * *
   
    Below the pin names for the
    S4D board.
    Use these pin names with
    Arduino commands like
    analogRead(), digitalRead(),
    analogWrite() and digitalWrite()
                                    
 * * * * * * * * * * * * * * * * * * */

const int VOLUMESENSOR = 0;    // analog input
const int MAGNETSENSOR = 1;    // analog input
const int POTENTIOMETER = 2;   // analog input
const int LIGHTSENSOR = 3;     // analog input

const int BUTTON1 = 4;         // digital input
const int BUTTON2 = 3;         // digital input

const int BUZZER = 6;          // digital output

const int LED_BLUE = 5;        // analog and digital output
const int LED_GREEN = 9;       // analog and digital output
const int LED_YELLOW = 10;     // analog and digital output
const int LED_RED = 11;        // analog and digital output

/* * * * * * * * * * * * * * * * * * *

  Use the following code to initialize
  the breadboard.

    initializeBreadboard()

  Always call initializeBreadboard()
  in your setup() function when using
  the breadboard.

* * * * * * * * * * * * * * * * * * * */

void initializeBreadboard() {
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);

  pinMode(VOLUMESENSOR, INPUT);
  pinMode(MAGNETSENSOR, INPUT);
  pinMode(POTENTIOMETER, INPUT);
  pinMode(LIGHTSENSOR, INPUT);
      
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);

  Serial.begin(9600);
  Serial.println("");
  playTone(440,50);
  
#ifndef NO_OLED 
  initializeOLED();
#endif
}

/* * * * * * * * * * * * * * * * * * *

  The code below creates the following
  command:

  playTone( frequency, duration )   // Play a tone on the buzzer. 
                                    // The frequency detemines how high the tone wil sound.
                                    //    A frequency of 0 produces silence.
                                    //    See https://pages.mtu.edu/~suits/notefreqs.html for 
                                    //    frequencies of musical notes.
                                    // Duration specifies the how long the note will sound (in
                                    //    milliseconds).
                                    // In contrast to the tone() function in Arduino library, this 
                                    //    function does not interfere with PWM on digital pins. But 
                                    //    it will only return after the duretion is complete, like delay().
    
* * * * * * * * * * * * * * * * * * */

void playTone(int frequency, int duration) {
  if(frequency == 0) {  // PAUSE
    delay(duration);
  } else {
    long wavePeriodInMicroSecs = 1.0 / frequency * 1000 * 1000;
    const long pauseDuration = 10000;
    for (long i = 0; i < duration * 1000L - pauseDuration; i += wavePeriodInMicroSecs) {
      digitalWrite(BUZZER, HIGH);
      delayMicroseconds(wavePeriodInMicroSecs / 2);
      digitalWrite(BUZZER, LOW);
      delayMicroseconds(wavePeriodInMicroSecs / 2);
    }
    delayMicroseconds(pauseDuration);  // very short pause so the listener can tell notes apart.
  }
}

/* * * * * * * * * * * * * * * * * * *

  The code below creates the following
  commands:
    
    OLED.clear()                      // Removes all pixels from the screen.
    OLED.copyToSerial()               // Everything printed to OLED will also be sent to the
                                      // Serial Monitor.
    
    OLED.print( value )               // Shows a value (text, number) on the screen.
    OLED.print( string, value )       // You can add a label or message before the value.

    OLED.printTop( value )            // Prints a smaller line in the upper half of the screen.
    OLED.printTop( string, value )    // You can add a label or message before the value.

    OLED.printBottom( value )         // Prints a smaller line in the lower half of the screen.
    OLED.printBottom( string, value ) // You can add a label or message before the value.

  The value for OLED.print() can be 
  a text or a number.

 * * * * * * * * * * * * * * * * * * *

  If you don't use the OLED, define
  the NO_OLED macro just before 
  including this header file. 
  Like this:

  #define NO_OLED
  #include "s4d_breadboard.h"

  This will exclude all OLED code 
  from the program, which can speed 
  up compile times, and save a lot 
  of memory.
  The OLED commands above will still
  work. They'll simply print to the
  Serial Monitor, as if 
  OLED.copyToSerial() had been called.
      
 * * * * * * * * * * * * * * * * * * */

#ifndef NO_OLED 
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

// This is some ugly code, required to get the display to work.
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0,
    /* reset=*/ U8X8_PIN_NONE,
    /* clock=*/ SCL,
    /* data=*/  SDA);
#endif

class OledClass {
  
  private:
#ifndef NO_OLED   
    bool mustCopyToSerial  = false;
#else    
    bool mustCopyToSerial  = true;  // if OLED disabled, send OLED output 
                                    // to Serial Monitor.
#endif
    bool lastPrintWasSmall = false; // if last print was half screen, 
                                    // next small print should clear 
                                    // only half the screen.
    String lastLines[2] = {"",""};
    
  public:
 
    OledClass() {
    }
    void copyToSerial() {
      mustCopyToSerial = true;
    }
    void printToSerial() {
      if( ! mustCopyToSerial ) {
        return;
      }
      int boxLength = max( lastLines[0].length(), lastLines[1].length() ) + 2;
      Serial.print( "+" );
      for(int i=0;i<boxLength;i++) {
        Serial.print('-');
      }
      Serial.println("+");
      Serial.print("| ");
      Serial.print( lastLines[0] );
      for(int i=0;i<boxLength-lastLines[0].length()-1;i++) {
        Serial.print(' ');
      }
      Serial.println("|");
      if( lastPrintWasSmall ) {
        Serial.print("| ");
        Serial.print( lastLines[1] );
        for(int i=0;i<boxLength-lastLines[1].length()-1;i++) {
          Serial.print(' ');
        }
        Serial.println("|");
      }
      Serial.print( "+" );
      for(int i=0;i<boxLength;i++) {
        Serial.print('-');
      }
      Serial.println("+");                        
    }
    void print(String text) {
#ifndef NO_OLED 
      char tempCharBuffer[20];
      text.toCharArray(tempCharBuffer, 20);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_helvR14_tr);
      u8g2.setFontPosBaseline();
      u8g2.drawStr(0, 20, tempCharBuffer);
      u8g2.sendBuffer();
#endif
      lastPrintWasSmall = false;
      lastLines[0] = text; 
      lastLines[1] = "";
      printToSerial();
    }
    void print(int number ) {
      print(String(number));
    }
    void print(long number ) {
      print(String(number));
    }
    void print(unsigned long number ) {
      print(String(number));
    }
    void print(double number ) {
      print(String(number));
    }
    void print(String label, String value) {
      String text = label + " " + value;
      print(text);
    }
    void print(String label, int number) {
      print(label, String(number));
    }
    void print(String label, long number) {
      print(label, String(number));
    }
    void print(String label, unsigned long number) {
      print(label, String(number));
    }
    void print(String label, double number) {
      print(label, String(number));
    }
    void printSmallLine(String text, int line) {  // line=0: top & line=1: bottom
#ifndef NO_OLED 
      char tempCharBuffer[30];
      text.toCharArray(tempCharBuffer, 30);
      if( lastPrintWasSmall ) {
        // just clear the line we're going to print on
        u8g2.setDrawColor(0);
        u8g2.drawBox( 0, 16*line, 128, 16);
        u8g2.setDrawColor(1);
      } else {   
        // clear the entire screen
        u8g2.clearBuffer();
      }
      u8g2.setFont(u8g2_font_helvR10_tr);
      u8g2.setFontPosBottom();
      int drawPos = line ? 32 : 15;
      u8g2.drawStr(0, drawPos, tempCharBuffer);
      u8g2.sendBuffer();
#endif
      lastLines[line] = text; 
      if( ! lastPrintWasSmall ) {
        lastLines[1-line] = ""; 
      }
      lastPrintWasSmall = true; 
      printToSerial();           
    }
    void printTop(String text) {
      printSmallLine( text, 0 );
    }
    void printTop(int number) {
      printSmallLine( String(number), 0 );
    }
    void printTop(long number) {
      printSmallLine( String(number), 0 );
    }
    void printTop(unsigned long number) {
      printSmallLine( String(number), 0 );
    }
    void printTop(double number ) {
      printSmallLine( String(number), 0 );
    }
    void printTop(String label, String value) {
      String text = label + " " + value;
      printSmallLine( text, 0 );
    }
    void printTop(String label, int number) {
      printTop(label, String(number));
    }
    void printTop(String label, long number) {
      printTop(label, String(number));
    }
    void printTop(String label, unsigned long number) {
      printTop(label, String(number));
    }
    void printTop(String label, double number) {
      printTop(label, String(number));
    }
    void printBottom(String text) {
      printSmallLine( text, 1 );
    }
    void printBottom(int number ) {
      printSmallLine(String(number), 1);
    }
    void printBottom(long number ) {
      printSmallLine(String(number), 1);
    }
    void printBottom(unsigned long number) {
      printSmallLine(String(number), 1);
    }
    void printBottom(double number ) {
      printSmallLine(String(number), 1);
    }
    void printBottom(String label, String value) {
      String text = label + " " + value;
      printSmallLine(text, 1);
    }
    void printBottom(String label, int number) {
      printBottom(label, String(number));
    }
    void printBottom(String label, long number) {
      printBottom(label, String(number));
    }
    void printBottom(String label, unsigned long number) {
      printBottom(label, String(number));
    }
    void printBottom(String label, double number) {
      printBottom(label, String(number));
    }
    void clear() {
      print("");
    }
};


OledClass OLED;

void initializeOLED() {
#ifndef NO_OLED 
  u8g2.begin();
#endif
  OLED.clear();
}

/* * * * * * * * * * * * * * * * * * *

  The code below creates the following
  command:
  
  VolumeSensor.read()
  
  Normal background noise should result in 
  about 48db. If VolumeSensor.read()
  returns a different value for silence
  (say, 60), you can adjust it like this:

    VolumeSensor.calibrationOffset = -12 

  (this would adjust value 60 to 48)
  
 * * * * * * * * * * * * * * * * * * */


class VolumeSensorClass {

  public:
    int micSamples = 512; 
    int calibrationOffset = 0; 


    int getAverageDB() {
      long totalSoundReadings = 0;

      for (int i = 0; i < micSamples; i++) {
        int reading = analogRead(VOLUMESENSOR);
        int amp = abs(reading - 512);
        totalSoundReadings += ((long)amp * amp);
      }

      int averageSoundReadings = totalSoundReadings / micSamples;
      float soundVolumeRMS = sqrt(averageSoundReadings); //rms = root mean squared sound pressure
      int dB = 20.0 * log10(soundVolumeRMS / 512) + 100 + calibrationOffset;
      return dB;
    }

    //Method returns volume. 
    int read() {
      analogReference(EXTERNAL);
      int averageDB = getAverageDB();
      if (averageDB != 0) {
        analogReference(INTERNAL);
        return averageDB;
      } else {
        return read();
      }
    }
};

VolumeSensorClass VolumeSensor;


#endif // S4D_BREADBOARD_H
