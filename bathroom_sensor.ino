#include "hs1527.h"
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
#define DIST_PIN A0
#define PIR_PIN 10
#define LED_PIN 9
#define AC_RF_PIN A2

#define DIST_THRESH 250
int ctr = 0;

HS1527 encoder(AC_RF_PIN);
char *address = "\xBE\xA3\x90";

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);  
  pinMode(DIST_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  //pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

boolean ledstate = true;

/*
void loop() {
  char motion = digitalRead(PIR_PIN);
  int dist = analogRead(DIST_PIN);
  
  if (dist > DIST_THRESH) {
    ctr = 20; 
  }
  
  if (ctr > 0) {
    digitalWrite(LED_PIN, HIGH); 
    ctr--;
  } else {
    digitalWrite(LED_PIN, LOW); 
  }
  delay(200);
}
*/
void loop() {
  int i = 0;
  //for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      digitalWrite(LED_PIN, HIGH);
      encoder.sendMsg(address, i);
      digitalWrite(LED_PIN, LOW);
    }
    //delay(0);
  //}
  
  delay(2000);
}
