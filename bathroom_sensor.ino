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
#define FLOOR_LED_PIN 2

#define DIST_THRESH 250
int ctr = -1;

boolean light_state = true;
#define LIGHT_OFF_NTICKS 40
#define LIGHT_ON_NTICKS 100
#define RF_CMD_NUM_REPEATS 30

HS1527 encoder(AC_RF_PIN);
char *address = "\xBE\xA3\x90";

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);  
  pinMode(DIST_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(FLOOR_LED_PIN, OUTPUT);
  
  delay(2000);
  overhead_lights(false);
  delay(1000);
}

boolean ledstate = true;

void cabinet_lights(boolean on) {
 digitalWrite(LED_PIN, on); 
}

void floor_lights(boolean on) {
 digitalWrite(FLOOR_LED_PIN, on); 
}

void overhead_lights(boolean on) {
  // Lights toggle, so start with a known state and 
  // use a boolean to track on/off
  if (on != light_state) {
    for (int i = 0; i < RF_CMD_NUM_REPEATS; i++) {
      encoder.sendMsg(address, 0); 
    }
  }
  light_state = on;
}

void loop() {
  char motion = digitalRead(PIR_PIN);
  int dist = analogRead(DIST_PIN);
  
  boolean motion_trigger = motion && (ctr == -LIGHT_OFF_NTICKS || ctr > 0);
  boolean distance_trigger = (dist > DIST_THRESH);
  
  if (distance_trigger) {
    cabinet_lights(true); 
  } else {
    cabinet_lights(false); 
  }
  
  if (motion_trigger || distance_trigger) {
    ctr = LIGHT_ON_NTICKS; 
    floor_lights(true);
    overhead_lights(true);
  } else if (ctr > -LIGHT_OFF_NTICKS) {
    ctr--; 
    floor_lights(false);
    overhead_lights(false);
  }
  
  delay(200);
}
