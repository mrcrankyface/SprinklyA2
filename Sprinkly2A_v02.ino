//Setup libraries
#include <TimeLib.h>
#include <LiquidCrystal.h>
/*
VSS till GND
VDD till PLUS
V0 till GND via 2k ohm
RS till A0
RW till GND
E till A1
D4 till A2
D5 till A3
D6 till A4
D7 till A5
A till PLUS via 220 ohm
K till GND
Extra knapp till pin1 för att spola klockan.
*/
const int rs = A0, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


/*Setup av pins
Vattenspridarsystem tänkt att arbeta efter tidur. */
const int clkfwd = 1;
const int s1 = 2;
const int s2 = 3;
const int s3 = 4;
const int s4 = 5;
const int s5 = 6;
const int s6 = 7;
const int s7 = 8;
const int s8 = 9;
const int powerPin = 10;
const int dataPin = 11;       // Pin DS on Relay board
const int latchPin =  12;     // Pin LT on Relay board
const int clockPin = 13;      // Pin CL on Relay board
unsigned long interval = 30;  // för skärmrensningen
byte data = 0;


//Minutkonstanten, 60s = 1 minut
unsigned long Minute = 60;


//Sprinklertider
/*int spr1short = 6*Minute; // Lilla pop-up i hörnet mot grusgården. 4'a.
int spr2short = 12*Minute; // Sektorspridare i häcken, kastar mot trallen. 5'a.
int spr3short = 9*Minute; // Stora pop-up spridaren i gräset. 6'a.
int spr4short = 9*Minute; // Sektorspridaren i häcken, kastar mot spabad. 7'a.
int spr5short = 1; // Ej i bruk;
int spr6short = 4*Minute; // Lillsprutan vid framsidan i häcken, startar 2'a
int spr7short = 9*Minute; // Sektorspridare i rabatt vid gräsklippare, kastar mot framsidan. 3'a.
int spr8short = 6*Minute; // Dubbla rotorer framsidan, startar 1'a

int spr1long = 30*Minute; // Lilla pop-up i hörnet mot grusgården. 4'a.
int spr2long = 60*Minute; // Sektorspridare i häcken, kastar mot trallen. 5'a.
int spr3long = 45*Minute; // Stora pop-up spridaren i gräset. 6'a.
int spr4long = 45*Minute; // Sektorspridaren i häcken, kastar mot spabad. 7'a.
int spr5long = 1; // Ej i bruk;
int spr6long = 20*Minute; // Lillsprutan vid framsidan i häcken, startar 2'a
int spr7long = 45*Minute; // Sektorspridare i rabatt vid gräsklippare, kastar mot framsidan. 3'a.
int spr8long = 30*Minute; // Dubbla rotorer framsidan, startar 1'a
*/

int spr1short = 10; // Lilla pop-up i hörnet mot grusgården. 4'a.
int spr2short = 10; // Sektorspridare i häcken, kastar mot trallen. 5'a.
int spr3short = 10; // Stora pop-up spridaren i gräset. 6'a.
int spr4short = 10; // Sektorspridaren i häcken, kastar mot spabad. 7'a.
int spr5short = 1; // Ej i bruk;
int spr6short = 10; // Lillsprutan vid framsidan i häcken, startar 2'a
int spr7short = 10; // Sektorspridare i rabatt vid gräsklippare, kastar mot framsidan. 3'a.
int spr8short = 10; // Dubbla rotorer framsidan, startar 1'a
int spr1long = 10; // Lilla pop-up i hörnet mot grusgården. 4'a.
int spr2long = 10; // Sektorspridare i häcken, kastar mot trallen. 5'a.
int spr3long = 10; // Stora pop-up spridaren i gräset. 6'a.
int spr4long = 10; // Sektorspridaren i häcken, kastar mot spabad. 7'a.
int spr5long = 1; // Ej i bruk;
int spr6long = 10; // Lillsprutan vid framsidan i häcken, startar 2'a
int spr7long = 10; // Sektorspridare i rabatt vid gräsklippare, kastar mot framsidan. 3'a.
int spr8long = 10; // Dubbla rotorer framsidan, startar 1'a



int spr1shortstate = 0;
int spr2shortstate = 0;
int spr3shortstate = 0;
int spr4shortstate = 0;
int spr5shortstate = 0;
int spr6shortstate = 0;
int spr7shortstate = 0;
int spr8shortstate = 0;

int spr1longstate = 0;
int spr2longstate = 0;
int spr3longstate = 0;
int spr4longstate = 0;
int spr5longstate = 0;
int spr6longstate = 0;
int spr7longstate = 0;
int spr8longstate = 0;

unsigned long startTime = 0;
unsigned long endTime = 0;
unsigned long currentTime = 0;


void setup() {
// put your setup code here, to run once:
Serial.begin(9600); //Initiera kommunikation
lcd.begin(20, 4);
lcd.setCursor(0,0);
lcd.print("Hello world!");
delay(500);

//Sätt alla pins
lcd.setCursor(0,2);
pinMode(latchPin, OUTPUT);
pinMode(dataPin,  OUTPUT);
pinMode(clockPin, OUTPUT);
pinMode(s1, INPUT_PULLUP);
pinMode(s2, INPUT_PULLUP);
pinMode(s3, INPUT_PULLUP);
pinMode(s4, INPUT_PULLUP);
pinMode(s5, INPUT_PULLUP);
pinMode(s6, INPUT_PULLUP);
pinMode(s7, INPUT_PULLUP);
pinMode(s8, INPUT_PULLUP);
pinMode(powerPin, OUTPUT);
digitalWrite(powerPin, LOW);
lcd.print("Pins setup...");
delay(500);

//Stäng av alla relän osv
digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
shiftOut(dataPin, clockPin, MSBFIRST, 0);     // Load the data to register using ShiftOut function
digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
lcd.setCursor(0,1);
lcd.print("Relayboard reset...");
delay(500);

//Start meddelande
lcd.setCursor(0,3);
lcd.print("Starting!");
setTime(9,59,56,1,1,1);  // TIIIIDEEEEEEEN
delay(1000);
lcd.clear();
}


// LOOOOOOOOOOOP BEGIIIIIIINS
void loop() {  

//Printkommandon för klockan.
if (hour() <= 9){
  lcd.setCursor(0, 0);  
  lcd.print(0);
  lcd.print(hour());
  }else{
  lcd.setCursor(0, 0);
  lcd.print(hour());
  }
if (minute() <= 9){
  lcd.setCursor(2, 0);
  lcd.print(":");
  lcd.print(0);
  lcd.print(minute());
  }else{
  lcd.setCursor(2, 0);
  lcd.print(":");
  lcd.print(minute());
  }
if (second() <= 9){
  lcd.setCursor(5, 0);
  lcd.print(":");
  lcd.print(0);
  lcd.print(second());
  lcd.print("       ");
  }else{
  lcd.setCursor(5, 0);
  lcd.print(":");
  lcd.print(second());
  lcd.print("       ");
  }
  lcd.setCursor(15, 0);
  lcd.print("Day:");
  lcd.setCursor(19,0);
  lcd.print(day());
if(day() == 1){
  lcd.setCursor(0,1);
  lcd.print("Short at 10, 14, 18 ");
  lcd.setCursor(0,2);
  lcd.print("Long at 20          ");
}
if(day() == 2){
  lcd.setCursor(0,1);
  lcd.print("Short at 10, 14, 18 ");
  lcd.setCursor(0,2);
  lcd.print("Long tomorrow.      ");
}


// Justera klockan, needs to change to while-loop so it doesn't get stuck when resets or sprinklers start.
int clkfwdstate = digitalRead(clkfwd);
if(clkfwdstate == LOW){
  adjustTime(60);
}

//Läs av brytare och visa symboler.
// SWITCH 1
int s1state = digitalRead(s1);
if (s1state == LOW){
  lcd.setCursor(0,3);
  lcd.print("1");
  }else{
  lcd.setCursor(0,3);
  lcd.print("x");
}
if (spr1longstate == 1){
  lcd.setCursor(1,3);
  lcd.print("L");
  }else if(spr1shortstate == 1){
  lcd.setCursor(1,3);
  lcd.print("S");
  }else{
  lcd.setCursor(1,3);
  lcd.print(" ");
}
// SWITCH 2
int s2state = digitalRead(s2);
if (s2state == LOW){
  lcd.setCursor(2,3);
  lcd.print("2");
  }else{
  lcd.setCursor(2,3);
  lcd.print("x");
}
if (spr2longstate == 1){
  lcd.setCursor(3,3);
  lcd.print("L");
  }else if(spr2shortstate == 1){
  lcd.setCursor(3,3);
  lcd.print("S");
  }else{
  lcd.setCursor(3,3);
  lcd.print(" ");
}
// SWITCH 3
int s3state = digitalRead(s3);
if (s3state == LOW){
  lcd.setCursor(4,3);
  lcd.print("3");
  }else{
  lcd.setCursor(4,3);
  lcd.print("x");
}
if (spr3longstate == 1){
  lcd.setCursor(5,3);
  lcd.print("L");
  }else if(spr3shortstate == 1){
  lcd.setCursor(5,3);
  lcd.print("S");
  }else{
  lcd.setCursor(5,3);
  lcd.print(" ");
}
// SWITCH 4
int s4state = digitalRead(s4);
if (s4state == LOW){
  lcd.setCursor(6,3);
  lcd.print("4");
  }else{
  lcd.setCursor(6,3);
  lcd.print("x");
}
if (spr4longstate == 1){
  lcd.setCursor(7,3);
  lcd.print("L");
  }else if(spr4shortstate == 1){
  lcd.setCursor(7,3);
  lcd.print("S");
  }else{
  lcd.setCursor(7,3);
  lcd.print(" ");
}
// SWITCH 5
int s5state = digitalRead(s5);
if (s5state == LOW){
  lcd.setCursor(8,3);
  lcd.print("5");
  }else{
  lcd.setCursor(8,3);
  lcd.print("x");
}
if (spr5longstate == 1){
  lcd.setCursor(9,3);
  lcd.print("L");
  }else if(spr5shortstate == 1){
  lcd.setCursor(9,3);
  lcd.print("S");
  }else{
  lcd.setCursor(9,3);
  lcd.print(" ");
}
// SWITCH 6
int s6state = digitalRead(s6);
if (s6state == LOW){
  lcd.setCursor(10,3);
  lcd.print("6");
  }else{
  lcd.setCursor(10,3);
  lcd.print("x");
}
if (spr6longstate == 1){
  lcd.setCursor(11,3);
  lcd.print("L");
  }else if(spr6shortstate == 1){
  lcd.setCursor(11,3);
  lcd.print("S");
  }else{
  lcd.setCursor(11,3);
  lcd.print(" ");
}
// SWITCH 7
int s7state = digitalRead(s7);
if (s7state == LOW){
  lcd.setCursor(12,3);
  lcd.print("7");
  }else{
  lcd.setCursor(12,3);
  lcd.print("x");
}
if (spr7longstate == 1){
  lcd.setCursor(13,3);
  lcd.print("L");
  }else if(spr7shortstate == 1){
  lcd.setCursor(13,3);
  lcd.print("S");
  }else{
  lcd.setCursor(13,3);
  lcd.print(" ");
}
// SWITCH 8
int s8state = digitalRead(s8);
if (s8state == LOW){
  lcd.setCursor(14,3);
  lcd.print("8");
  }else{
  lcd.setCursor(14,3);
  lcd.print("x");
}
if (spr8longstate == 1){
  lcd.setCursor(15,3);
  lcd.print("L");
  }else if(spr8shortstate == 1){
  lcd.setCursor(15,3);
  lcd.print("S");
  }else{
  lcd.setCursor(15,3);
  lcd.print(" ");
}
// SWITCHES DONE!

// Clean screen every 45 sec.
unsigned long currentSec = millis()/1000;
if (currentSec > interval){
  delay(50);
  lcd.clear();
  interval = interval + 45;
  }else{
  delay(50);
}

/* water session data
Data 1 = sektion 8
Data 4 = sektion 6(egentligen 7?)
Data 2 = sektion 7(egentligen 6?)
Data 128 = sektion 1
Data 64 = sektion 2
Data 32 = sektion 3
data 16 = sektion 4
data 8 = sektion 5
*/

//SEKTION 8 LONG
while(hour() >= 20 && s8state == LOW && spr8longstate == 0 && day() == 1 || hour() <= 6 && s8state == LOW && spr8longstate == 0 && day() == 2){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 1;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 8L started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr8long;
    currentTime = startTime;
  }
    
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("8L Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(15,3);
    lcd.print("L");
    spr8longstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);
  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit in the while-loop.
}

//SEKTION 7 LONG
while(hour() >= 20 && s7state == LOW && spr7longstate == 0 && day() == 1 || hour() <= 6 && s7state == LOW && spr7longstate == 0 && day() == 2){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 4;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 7L started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr7long;
    currentTime = startTime;
  }
    
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("7L Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(13,3);
    lcd.print("L");
    spr7longstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);
  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit in the while-loop.
}

//SEKTION 6 LONG
while(hour() >= 20 && s6state == LOW && spr6longstate == 0 && day() == 1 || hour() <= 6 && s6state == LOW && spr6longstate == 0 && day() == 2){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 2;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 6L started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr6long;
    currentTime = startTime;
  }
    
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("6L Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(11,3);
    lcd.print("L");
    spr6longstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);
  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit in the while-loop.
}

//SEKTION 5 LONG
while(hour() >= 20 && s5state == LOW && spr5longstate == 0 && day() == 1 || hour() <= 6 && s5state == LOW && spr5longstate == 0 && day() == 2){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 8;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 5L started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr5long;
    currentTime = startTime;
  }
    
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("5L Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(9,3);
    lcd.print("L");
    spr5longstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);
  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit in the while-loop.
}

//SEKTION 1 LONG
while(hour() >= 20 && s1state == LOW && spr1longstate == 0 && day() == 1 || hour() <= 6 && s1state == LOW && spr1longstate == 0 && day() == 2){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 128;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 1L started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr1long;
    currentTime = startTime;
  }
    
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("1L Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(1,3);
    lcd.print("L");
    spr1longstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);
  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit in the while-loop.
}

//SEKTION 2 LONG
while(hour() >= 20 && s2state == LOW && spr2longstate == 0 && day() == 1 || hour() <= 6 && s2state == LOW && spr2longstate == 0 && day() == 2){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 64;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 2L started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr2long;
    currentTime = startTime;
  }
    
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("2L Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(3,3);
    lcd.print("L");
    spr2longstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);
  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit in the while-loop.
}

//SEKTION 3 LONG
while(hour() >= 20 && s3state == LOW && spr3longstate == 0 && day() == 1 || hour() <= 6 && s3state == LOW && spr3longstate == 0 && day() == 2){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 32;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 3L started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr3long;
    currentTime = startTime;
  }
    
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("3L Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(5,3);
    lcd.print("L");
    spr3longstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);
  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit in the while-loop.
}

//SEKTION 4 LONG
while(hour() >= 20 && s4state == LOW && spr4longstate == 0 && day() == 1 || hour() <= 6 && s4state == LOW && spr4longstate == 0 && day() == 2){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 16;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 4L started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr4long;
    currentTime = startTime;
  }
    
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("4L Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(7,3);
    lcd.print("L");
    spr4longstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);
  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit in the while-loop.
}
// LONG SESSIONS DONE!



// Short water sessions
// SEKTION 8 SHORT
while(hour() >= 10 && hour() <= 18 && s8state == LOW && spr8shortstate == 0){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 1;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 8S started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr8short;
    currentTime = startTime;
  }
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("8S Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(15,3);
    lcd.print("S");
    spr8shortstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);
  }
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit
}

// SEKTION 7 SHORT
while(hour() >= 10 && hour() <= 18 && s7state == LOW && spr7shortstate == 0){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 4;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 7S started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr7short;
    currentTime = startTime;
  }
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("7S Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(13,3);
    lcd.print("S");
    spr7shortstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);
  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit
}

// SEKTION 6 SHORT
while(hour() >= 10 && hour() <= 18 && s6state == LOW && spr6shortstate == 0){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 2;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs

    lcd.setCursor(0,0);
    lcd.print("Section 6S started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr6short;
    currentTime = startTime;
  }
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("6S Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(11,3);
    lcd.print("S");
    spr6shortstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);

  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit
}

// SEKTION 5 SHORT
while(hour() >= 10 && hour() <= 18 && s5state == LOW && spr5shortstate == 0){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 8;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 5S started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr5short;
    currentTime = startTime;
  }
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("5S Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(9,3);
    lcd.print("S");
    spr5shortstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);
  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit
}

// SEKTION 1 SHORT
while(hour() >= 10 && hour() <= 18 && s1state == LOW && spr1shortstate == 0){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 128;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 1S started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr1short;
    currentTime = startTime;
  }
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("1S Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(1,3);
    lcd.print("S");
    spr1shortstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);
  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit
}

// SEKTION 2 SHORT
while(hour() >= 10 && hour() <= 18 && s2state == LOW && spr2shortstate == 0){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 64;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 2S started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr2short;
    currentTime = startTime;
  }
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("2S Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(3,3);
    lcd.print("S");
    spr2shortstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);
  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit
}

// SEKTION 3 SHORT
while(hour() >= 10 && hour() <= 18 && s3state == LOW && spr3shortstate == 0){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 32;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 3S started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr3short;
    currentTime = startTime;
  }
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("3S Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(5,3);
    lcd.print("S");
    spr3shortstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);
  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit
}

// SEKTION 4 SHORT
while(hour() >= 10 && hour() <= 18 && s4state == LOW && spr4shortstate == 0){
  if(data == 0){ // runs once
    digitalWrite(powerPin, HIGH);
    delay(1505);
    data = 16;
    digitalWrite(latchPin, LOW);                  // Latch is low while we load new data to register
    shiftOut(dataPin, clockPin, MSBFIRST, data);  // Load the data to register using ShiftOut function
    digitalWrite(latchPin, HIGH);                 // Toggle latch to present the new data on register outputs
    lcd.setCursor(0,0);
    lcd.print("Section 4S started. ");
    startTime = (millis() / 1000);
    endTime = startTime + spr4short;
    currentTime = startTime;
  }
  if(endTime > currentTime){ // keeps running until currentTime passes endTime
    currentTime = (millis() / 1000);
    lcd.setCursor(0,1);
    lcd.print("Time remaining:     ");
    lcd.setCursor(0,2);
    float countDown = endTime-currentTime;
    lcd.print(countDown/60);
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print(" minutes.");
  }else{
    lcd.setCursor(0,0);
    lcd.print("4S Done!            ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(7,3);
    lcd.print("S");
    spr4shortstate = 1; // sets state to 1 once endTime is passed, kicks it out of the while-loop.
    data = 0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, data);
    digitalWrite(latchPin, HIGH);
    digitalWrite(powerPin, LOW);
    delay(751);

  }
    
  s1state = digitalRead(s1); // read the switch every loop
  s2state = digitalRead(s2); // read the switch every loop
  s3state = digitalRead(s3); // read the switch every loop
  s4state = digitalRead(s4); // read the switch every loop
  s5state = digitalRead(s5); // read the switch every loop
  s6state = digitalRead(s6); // read the switch every loop
  s7state = digitalRead(s7); // read the switch every loop
  s8state = digitalRead(s8); // read the switch every loop
  delay(50); // Calm things down a bit
}


// Turn off relays if data =/= 0.
if(data != 0){
  data = 0;
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);
}


// LONG RESET ...
// Timer resets at 8 so long watering can start later.
int longstates = spr1longstate+spr2longstate+spr3longstate+spr4longstate+spr5longstate+spr6longstate+spr7longstate+spr8longstate;
if(hour() >= 8 && hour() <= 18 && longstates > 0){
  lcd.setCursor(0,0);
  lcd.print("Reset long  timer   ");
  lcd.setCursor(0,1);
  lcd.print("                    ");
  lcd.setCursor(0,2);
  lcd.print("                    ");
  spr1longstate = 0;
  spr2longstate = 0;
  spr3longstate = 0;
  spr4longstate = 0;
  spr5longstate = 0;
  spr6longstate = 0;
  spr7longstate = 0;
  spr8longstate = 0;
  delay(2001);
}


// SHORT RESETS...
// First start is 12.
// Timer reset 14 for short watering.
// Timer resets 18 for short watering.
// Timer reset at 19(after allowed short-time) so short watering can start later.
int shortstates = spr1shortstate+spr2shortstate+spr3shortstate+spr4shortstate+spr5shortstate+spr6shortstate+spr7shortstate+spr8shortstate;
if(hour() == 14 && minute() == 0 && shortstates > 0 || hour() == 18 && minute() == 0 && shortstates > 0 || hour() == 19 && shortstates > 0){
  lcd.setCursor(0,0);
  lcd.print("Reset short timer   ");
  lcd.setCursor(0,1);
  lcd.print("                    ");
  lcd.setCursor(0,2);
  lcd.print("                    ");
  spr1shortstate = 0;
  spr2shortstate = 0;
  spr3shortstate = 0;
  spr4shortstate = 0;
  spr5shortstate = 0;
  spr6shortstate = 0;
  spr7shortstate = 0;
  spr8shortstate = 0;
  delay(2001);
}


//Reset day3 to day1.
if(day() == 3){
  adjustTime(-172800);
}


}
