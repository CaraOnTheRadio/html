#include "s4d_breadboard.h"

void setup() {
  initializeBreadboard();
}

String dayNames[] = {
  "Sunday",      // 0    use these numbers in your boolean expressions, not the strings
  "Monday",      // 1
  "Tuesday",     // 2
  "Wednesday",   // 3
  "Thursday",    // 4
  "Friday",      // 5
  "Saturday"     // 6
};

void loop() {

  // use helper function for reading the hour from potMeter
  int hour = getHour();
  // use helper function to deal with buttons for changing the weekday
  int dayNumber = adjustWeekday();
  // use the dayName only for printing, not in your boolean expressions (see line 8)
  String dayName = dayNames[dayNumber];
  OLED.printTop( dayName, String(hour) + ":00" );


  // TODO: Replace the 'false' values with boolean expressions
  //       that reflect their names.
  // NOTE: Feel free to use some variables in the expressions of
  //       other variables.


  print(dayNumber);

  bool isSleeping = (hour >= 0 && hour < 8);
  bool havingDinner = ( hour >= 18 && hour < 19);
  bool isWeekend = ( dayNumber == 6 || dayNumber == 0);
  bool mustWork = !isWeekend;
  bool isWorkingDay = (hour >=8 && hour < 18 && mustWork);
  bool doingSport = ( 19 <= hour && 22 > hour && (1 == dayNumber || 3 == dayNumber));
  bool isFree = (!isSleeping && !havingDinner && !isWeekend && !mustWork && !isWorkingDay && !doingSport);




if( isSleeping )   { OLED.printBottom( "Sleeping. Zzzz..."); }
if( havingDinner ) { OLED.printBottom( "Dinner with family" ); }
if( mustWork )     { OLED.printBottom( "Working: busy,busy" ); }
if( doingSport )   { OLED.printBottom( "To the fitness club." ); }
if( isFree )       { OLED.printBottom( "Playstation time!" ); }


}

////////  HELPER FUNCTIONS:  ///////////////////////////////

int getHour() {
  return analogRead( POTENTIOMETER ) / 43;   // range 0 - 1023 => range 0 - 23
}

bool isButtonPressed(int buttonPin) {
  if( digitalRead(buttonPin) ) {
    delay(200);  // wait a very short time for user to release button
    return true;
  } else {
    return false;
  }
}

int weekday = 1;  // Start on Monday

int adjustWeekday() {
  if( isButtonPressed(BUTTON1) ) {
    weekday = weekday + 1;
    if( weekday > 6 ) {
      weekday = 0;
    }
  }
  if( isButtonPressed(BUTTON2) ) {
    weekday = weekday - 1;
    if( weekday < 0 ) {
      weekday = 6;
    }
  }
  return weekday;
}
