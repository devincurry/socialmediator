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

  "'If you are lonely  ",
  "'Solitude is        ",
  "'If you make friends",
  "'I never found a    ",
  "'He who lives in    ",
  "'The worst solitude ",
  "'The best time to   ",
  "'Things are never   ",
  "'Only a life lived  "
};

char* line1[]={
  //  "'If you are lonely", "when you're alone,", "you are in bad comp-",
  //  "any.' J.P. Sartre"};

  "when you are alone,",
  "independence.'      ",
  "with yourself you   ",
  "companion that was  ",
  "solitude may make   ",
  "is to have no real  ",
  "make friends is     ",
  "quite as scary when ",
  "for others is a life"
};

char* line2[]={
  //  "'If you are lonely", "when you're alone,", "you are in bad comp-",
  //  "any.' J.P. Sartre"};

  "you are in bad comp-",
  "-Herman Hesse       ",
  "will never be       ",
  "so companionable as ",
  "his own laws.'      ",
  "friendships.'       ",
  "before you need     ",
  "you've got a best   ",
  "worthwhile.'        "
};

char* line3[]={
  //  "'If you are lonely", "when you're alone,", "you are in bad comp-",
  //  "any.' J.P. Sartre"};

  "-any.' -J.P. Sartre ",
  "                    ",
  "alone.' -M. Maltz   ",
  "solitude.' -Thoreau ",
  "-Pubilius Syrus     ",
  "-Francis Bacon      ",
  "them.' -E. Barrymore",
  "friend.' -Watterson ",
  "-Albert Einstein    "
};

int totalStrings = 9;




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
    if (analogRead(A1) >= 1000 && analogRead(A2) >= 1000) {
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


