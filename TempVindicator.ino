#include <FastLED.h>

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
int menu = 0;
int rtcSlot = 0;                                          // rtcSlot - rtcSlot + 5 are addresses of date and time  


void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(  10 );                          // default brightness
  leds[0] = CRGB(0, 255, 0);
  leds[1] = CRGB(0, 255, 0);
  FastLED.show();
  
  rtcStart();
  rtcSet(11, 10, 17, 18, 48);                            // set rtc by sending day, month, year, hour(24hr format), minute
  storeTime(rtcSlot);
  
  lcdStart();
  lanStart();
}

void loop() {
  inKey = keypad();
  tempArr = takeReading();                                // bring in temperature from tempSensor
  showThresholds(tempArr[0], tempArr[1]);                 // compare temperature to thresholds
  interface();
  
  lanPacketLoop();
}
