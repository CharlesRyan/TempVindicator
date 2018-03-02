#include <FastLED.h>
#include <UIPEthernet.h>
#include <UIPServer.h>
#include <UIPClient.h>
#include "utility/logging.h"

// RGB setup 
#define NUM_LEDS 2
#define DATA_PIN A2
CRGB leds[NUM_LEDS];

float temp1;
float temp2;
float *tempArr;
int inKey;
int curKey;
int lastKey;
int menu = 1; 

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(  10 );                          // default brightness
  leds[0] = CRGB(0, 255, 0);
  leds[1] = CRGB(0, 255, 0);
  FastLED.show();
  
  rtcStart();
//  rtcSet(11, 10, 17, 18, 48);                            // set rtc by sending day, month, year, hour(24hr format), minute
//  storeTime(getTimeSlot());
  lcdStart();
  lanStart();
}

void loop() {
  clientWatch();
  inKey = keypad();                                       // process keypad input
  interface();                                            // take action based on keypad input
  tempArr = takeReading();                                // bring in temperature from tempSensor
  showThresholds(tempArr[0], tempArr[1]);                 // compare temperature to thresholds
}
