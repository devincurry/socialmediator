// include the library code:
#include <Servo.h>      // include the servo library
#include <LiquidCrystal.h>
#include <Wire.h>
#define REDLITE 3
#define GREENLITE 5
#define BLUELITE 6

//LCD CODE

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
// you can change the overall brightness by range 0 -> 255
int brightness = 255;

int row;
int col;

int currentQuoteIndex = 0;
int line1Index = 0;
int line2Index = 0;
int line3Index = 0;
unsigned long lastPushed;
unsigned long lastLocked;
int lockSeconds;
int lockMinutes;
int lockHours;

boolean quoteOn = false;

//SERVO CODE
Servo myServo;       // creates an instance of the servo object to control a servo
int servoPin = 2;       // Control pin for servo motor
int button = A2;  // analog pin used to connect the potentiometer
int val = 90;    // variable to read the value from the analog pin

boolean doorOpen = false;
boolean doorLocked = false;

char* line0[]={
  //  "'If you are lonely", "when you're alone,", "you are in bad comp-",
  //  "any.' J.P. Sartre"};
  /*
  "'If you are lonely  ", //00
   "'Solitude is        ", //01
   "'If you make friends", //02
   
   "'I never found a    ", //03
   
   "'He who lives in    ", //04
   */
  "'The worst solitude ", //05
  "'The best time to   ", //06
  "'Things are never   ", //07
  "'Only a life lived  ", //08
  "'Things do not      ", //09
  "'Either you run the ", //10

  "'Act as if what you ", //11
  "'Quality is not an  ", //12
  "'What you do today  ", //13
  "'Do something wond- ", //14
  "'It's always too    ", //15
  "'Don't watch the    ", //16
  "'Men have become the", //17
  "'Technology is a    ", //18
  "'Treasure your rel- ", //19
  "'We like technology "  //20

};

char* line1[]={
  //  "'If you are lonely", "when you're alone,", "you are in bad comp-",
  //  "any.' J.P. Sartre"};
  /*
  "when you are alone, ", //00
   "independence.'      ", //01
   "with yourself you   ", //02
   
   "companion that was  ", //03
   
   "solitude may make   ", //04
   */
  "is to have no real  ", //05
  "make friends is     ", //06
  "quite as scary when ", //07
  "for others is a life", //08
  "happen. Things are  ", //09
  "day or the day runs ", //10

  "do makes a diff-    ", //11
  "act, it is a habit.'", //12
  "can improve all your", //13
  "erful, people may   ", //14
  "early to quit.'     ", //15
  "clock; do what it   ", //16
  "tools of their      ", //17
  "useful servant but a", //18
  "ationships, not your", //19
  "because we don't    "  //20

};

char* line2[]={
  //  "'If you are lonely", "when you're alone,", "you are in bad comp-",
  //  "any.' J.P. Sartre"};
  /*
  "you are in bad comp-", //00
   "-Herman Hesse       ", //01
   "will never be       ", //02
   
   "so companionable as ", //03
   
   "his own laws.'      ", //04
   */
  "friendships.'       ", //05
  "before you need     ", //06
  "you've got a best   ", //07
  "worthwhile.'        ", //08
  "made to happen.'    ", //09
  "you.' -Jim Rohn     ", //10

  "erence. It does.'   ", //11
  "-Aristotle          ", //12
  "tomorrows.'         ", //13
  "imitate it.'        ", //14
  "-Norman Vincent     ", //15
  "does. Keep going.'  ", //16
  "tools.' -Henry David", //17
  "dangerous master.'  ", //18
  "possessions.'       ", //19
  "have to talk to any-"  //20

};

char* line3[]={
  //  "'If you are lonely", "when you're alone,", "you are in bad comp-",
  //  "any.' J.P. Sartre"};
  /*
  "-any.' -J.P. Sartre ", //00
   "                    ", //01
   "alone.' -M. Maltz   ", //02
   
   "solitude.' -Thoreau ", //03
   
   "-Pubilius Syrus     ", //04
   */
  "-Francis Bacon      ", //05
  "them.' -E. Barrymore", //06
  "friend.' -Watterson ", //07
  "-Albert Einstein    ", //08
  "-John F. Kennedy    ", //09
  "                    ", //10

  "-William James      ", //11
  "                    ", //12
  "-Ralph Marston      ", //13
  "-Albert Schweitzer  ", //14
  "Peale               ", //15
  "-Sam Levenson       ", //16
  "Thoreau             ", //17
  "-Christian L. Lange ", //18
  "-Anthony J. D'Angelo", //19
  "body.' -H. Rheingold"  //20

};

int totalStrings = 16;




void setup() {
  // set up the LCD's number of rows and columns:
  lcd.begin(20, 4);
  Serial.begin(9600);

  /*
    // Print a message to the LCD.
   lcd.print("RGB 20x4 Display ");
   lcd.setCursor(0,1);
   lcd.print(" Multicolor LCD ");
   lcd.setCursor(0,2);
   lcd.print("  This shit works!");
   lcd.setCursor(0,3);
   lcd.print("   FUCK YEAH!");
   */
  pinMode(REDLITE, OUTPUT);
  pinMode(GREENLITE, OUTPUT);
  pinMode(BLUELITE, OUTPUT);
  brightness = 100;

  //SERVO PINZ
  pinMode(13, INPUT); //button
  myServo.attach(servoPin);  // attaches the servo on pin 2 to the servo object


}

void loop() {
  screen();
  door();

  if (doorLocked == true) {

    //    lockSeconds = (millis()-lastLocked)/1000;
    timer();
  }
}

//SCREEN FUNCTIONS
void screen(){
  if (digitalRead(13) == HIGH) {
    if (quoteOn == false) {
      delay(200);
      quoteOn = true;
    } 
    else {
      delay(200);
      currentQuoteIndex ++;
      if (currentQuoteIndex >= totalStrings) {
        currentQuoteIndex = 0; 
      }
    }    
    lastPushed = millis();
  }

  if (quoteOn == true && millis() - lastPushed > 4000) {
    quoteOn = false;
    //    lockSeconds = (millis()-lastLocked)/1000;
    //timer();
  } 

  if (quoteOn) {
    displayText();
  } 
  else {
    blankScreen();
  }
}

void setBacklight(uint8_t r, uint8_t g, uint8_t b) {
  // normalize the red LED - its brighter than the rest!
  r = map(r, 0, 255, 0, 100);
  g = map(g, 0, 255, 0, 150);
  r = map(r, 0, 255, 0, brightness);
  g = map(g, 0, 255, 0, brightness);
  b = map(b, 0, 255, 0, brightness);
  // common anode so invert!
  r = map(r, 0, 255, 255, 0);
  g = map(g, 0, 255, 255, 0);
  b = map(b, 0, 255, 255, 0);
  Serial.print("R = "); 
  Serial.print(r, DEC);
  Serial.print(" G = "); 
  Serial.print(g, DEC);
  Serial.print(" B = "); 
  Serial.println(b, DEC);
  analogWrite(REDLITE, r);
  analogWrite(GREENLITE, g);
  analogWrite(BLUELITE, b);
}

void displayText() {

  // Print a message to the LCD.
  col = 0;
  row = 0;

  lcd.setCursor(col,0);
  lcd.print(line0[currentQuoteIndex]);

  lcd.setCursor(col,1);
  lcd.print(line1[currentQuoteIndex]);

  lcd.setCursor(col,2);
  lcd.print(line2[currentQuoteIndex]);

  lcd.setCursor(col,3);
  lcd.print(line3[currentQuoteIndex]);  

}

void blankScreen() {
  //  for (int r; r < 4; r++)
  col = 0;
  lcd.setCursor(col,0);
  lcd.print("                    ");
  lcd.setCursor(col,1);
  lcd.print("                    ");
  lcd.setCursor(col,2);
  lcd.print("                    ");
  lcd.setCursor(col,3);
  lcd.print("                    ");

}

//SERVO FUNCTIONS


void door() {

  //open with Open button
  if (doorOpen == false && doorLocked == false) {
    if(digitalRead(13) == HIGH) {
      openDoor();
    }

    //open with Special buttons
    if (analogRead(A1) >= 1000 && analogRead(A2) >= 1000) {
      openDoor();
    }

    //lock door
    if (analogRead(A0) >= 1000) {
      lockDoor();
    }
  }

  if (doorOpen == false && doorLocked == true) {
    //  if (analogRead(A1) >= 1000 && analogRead(A2) >= 1000) {
    if (analogRead(A1) >= 1000 && analogRead(A2) >= 1000 && analogRead(A3) >= 1000 && analogRead(A4) >= 1000 && analogRead(A5) >= 1000) {
      openDoor();
    }
  }

  if (doorOpen == true) {
    if(digitalRead(13) == LOW) {
      openDoor();
    }
    if (digitalRead(13) == HIGH) {
      closeDoor();
    }
  }
  // Serial.print(digitalRead(13));
}

void openDoor() {
  myServo.write(val);  // sets the servo position according to the scaled value
  delay(400);
  doorOpen = true;
  doorLocked = false;
  quoteOn = false;
}

void closeDoor() {
  myServo.write(0);
  delay(400);
  doorOpen = false;
}

void lockDoor() {
  doorLocked = true;
  quoteOn = true;
  lastLocked = millis();
  timer();
}

void timer() {

  lockSeconds = (((millis()-lastLocked)/1000));

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  if (quoteOn == false) {
    lcd.setCursor(3, 1);
    lcd.print("Timer");
    lcd.setCursor(10,1);
    lcd.print("0");
    lcd.setCursor(11,1);
    lcd.print(lockHours);
    lcd.setCursor(12,1);
    lcd.print(":");

    if (lockMinutes < 10) {
      lcd.setCursor(13,1);
      lcd.print("0");
      lcd.setCursor(14,1);
      lcd.print(lockMinutes);
    }

    if (lockMinutes > 9 && lockMinutes < 60) {
      lcd.setCursor(13,1);
      lcd.print(lockMinutes);
    }
  }

  if (lockSeconds >= 60) {
    lockMinutes = lockMinutes + 1;
    lastLocked = millis();
    lockSeconds = (((millis()-lastLocked)/1000));
  }


  if (lockMinutes >= 60) {
    lockMinutes = lockMinutes - 60;
    lockHours = lockHours + 1;
  }
}



