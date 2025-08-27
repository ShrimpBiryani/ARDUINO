/*
National Anthem of India on Custom Breadboard Arduino with ATmega328P
By Anay Narendra Kamat (http://www.anaykamat.com)
Modified for LCD + Servo + Buzzer demo
*/

// -------------------- Libraries --------------------
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// -------------------- Pin Config -------------------
int speakerPin = 10;
int servoPin   = 9;
int potPin     = A0;

// -------------------- Objects ----------------------
Servo Servo1;
LiquidCrystal_I2C lcd(0x27, 16, 2);   // Change 0x27 if your scanner finds a different address

// -------------------- Music Data -------------------
char notes[] = "cdeeeeeeeeedef eeedddbdc ccgggggggggaf fffffeddf eedeedeggaff eeeeeddbdc cdeeeedf efgggfedfe feeeddbdc ccggggeggggga fffffedfe egc bab aga ccddeedef "; 
int beats[] = { 1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,
                2,1,1,2,1,1,1,1,1,1,
                2,2,1,2,1,1,1,1,1,1,1,2,1,1,
                2,1,1,2,1,1,1,1,1,1,
                2,1,1,2,1,1,1,1,2,1,1,1,1,
                2,1,1,1,1,1,1,1,1,2,1,
                1,1,1,1,2,2,2,1,1,
                1,1,1,1,2,1,1,1,1,1,1,
                2,2,1,1,1,1,1,1,1,1,
                1,1,1,1,2,1,1,2,1,1,1,1,1,1,
                2,1,1,2,1,1,1,1,1,1,
                1,1,4,1,1,1,4,1,1,1,4,1,
                1,1,1,1,1,1,1,1,4,1 };
int length = sizeof(beats) / sizeof(beats[0]);  // number of notes
int tempo = 300;

// -------------------- Functions --------------------
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[]  = {1915,1700,1519,1432,1275,1136,1014,956};
  
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
      return;
    }
  }
}

// -------------------- Setup ------------------------
void setup() {
  pinMode(speakerPin, OUTPUT);
  Servo1.attach(servoPin);

  lcd.init();        // Initialize LCD
  lcd.backlight();   // Turn on backlight
  lcd.setCursor(0,0);
  lcd.print("Jai Shree Ram");
  delay(2000);       // Show greeting for 2s
  lcd.clear();
}

// -------------------- Main Loop --------------------
void loop() {
  // Read potentiometer and move servo
  int reading = analogRead(potPin);
  int angle = map(reading, 0, 1023, 0, 180);
  Servo1.write(angle);

  // Play anthem
  for (int i = 0; i < length; i++) {
    // Update servo during playback
    int reading = analogRead(potPin);
    int angle = map(reading, 0, 1023, 0, 180);
    Servo1.write(angle);

    // Update LCD
    lcd.setCursor(0,0);
    lcd.print("Jai Shree Ram   ");  // first line
    lcd.setCursor(0,1);
    lcd.print("Note: ");           
    lcd.print(i+1);                 // show current note number
    lcd.print("/");                 
    lcd.print(length);

    // Play note
    if (notes[i] == ' ') {
      delay(beats[i] * tempo);  // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }

    delay(tempo/2);  // short pause
  }

  // Song finished
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Anthem Done!");
  delay(5000);
}

