
#include "Arduino.h"
#include <Servo.h> 
#include <inttypes.h>
#include <ESP8266WiFi.h>

#define MILISECONDS_PER_DAY 86400000

#define SSID "tates telefone"
#define PASSWORD "jkjk"

#include "network.h"

Servo feedServo; 


void feedFish(uint8_t& slotNum, int8_t initialPosition = -180){
  // 360 / 9 = 40 degrees
  feedServo.write(40 * slotNum);
  delay(500);

  feedServo.write(initialPosition);

  slotNum++;
  
}


void setup(){
  
  feedServo.attach(A3); 
  feedServo.write(-180);

  Serial.begin(115200);
  
  wifiInit();
  serverInit();
  
} 


void loop(){ 
  
  static uint32_t msRemaining = MILISECONDS_PER_DAY;
  static uint8_t slot = 0;

  // check the user input from the internet
  if (checkInput(msRemaining, (slot <= 5))) {
    feedFish(slot);
    msRemaining = MILISECONDS_PER_DAY;
    return;    
  }
  
  
  // wait 
  delay(5);  
  msRemaining -= 5;

  
  if (msRemaining == 0) {
    feedFish(slot);
    msRemaining = MILISECONDS_PER_DAY;
    return; 
  }
  
}
