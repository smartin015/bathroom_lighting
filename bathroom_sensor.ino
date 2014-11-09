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
#define FLOOR_LED_PIN 5

#define DIST_THRESH 250
int RF_ctr = -1; // Counter for 
int IR_ctr = -1; // Counter for IR distance sensor + mirror lights

boolean light_state = true;
#define LIGHT_OFF_NTICKS 40
#define LIGHT_ON_NTICKS 100
#define RF_CMD_NUM_REPEATS 5
#define MIRROR_ON_NTICKS 10

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

int floor_light_pwm = 0;
void floor_lights(boolean on) {
 if (on) {
     analogWrite(FLOOR_LED_PIN, floor_light_pwm);
 }
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
  
  boolean motion_trigger = motion && (RF_ctr == -LIGHT_OFF_NTICKS || RF_ctr > 0);
  boolean distance_trigger = (dist > DIST_THRESH);
  
  if (distance_trigger || IR_ctr > 0) {
    cabinet_lights(true); 
    IR_ctr = MIRROR_ON_NTICKS;
  } else {
    cabinet_lights(false); 
  }
  if (IR_ctr > 0) {
    IR_ctr--; 
  }
  
  if (motion_trigger || distance_trigger) {
    RF_ctr = LIGHT_ON_NTICKS; 
    floor_lights(true);
    overhead_lights(true);
  } else if (RF_ctr > -LIGHT_OFF_NTICKS) {
    RF_ctr--; 
    
    if (RF_ctr < 0) {
      floor_lights(false);
      overhead_lights(false);
    }
  }
  
  delay(200);
}
