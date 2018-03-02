#include <SimpleDHT.h>
#include <EEPROM.h>

// Temp/Hum setup
int pinDHT1 = 2;              // Temp sensor 1 setup at pin 2
int pinDHT2 = A3;              // Temp sensor 2 setup at analog pin 3
SimpleDHT22 dht22;

int err;                       // DHT22
int tempIdx = 0;               //starting index in EEPROM
int bestIdx;                   // index of high or low value during search
int tHead = 0;                 // pointer to oldest temp value in EEPROM
int tTail = 0;                 // tail that tracks newest temp addition to EEPROM  
int headTime = 96;             // 196: Year, 197: Month, 198: Day, 199: Hour, 200: Minute
static byte chvec = 0;         // vector used in recordStats()
float curTemp1;                 // updated after every period of queryDelay 
float curTemp2;                 // updated after every period of queryDelay 
float output[2];
int queryDelay = 1000;         // interval after which curTemp is updated
int writeDelay = 2000;

void nuke(){
   for( int k = 0; k < 96; k++) {
    EEPROM.write(k, 0);
  }
}

int checkLimits(float temp, int sensor) {
  if(temp >= 98){             // MjOv
    Serial.print("Sensor ");
    Serial.print(sensor);
    Serial.println(" Major Over ");
    return 1;
  } else if(temp >= 89) {     // MnOv
    Serial.print("Sensor ");
    Serial.print(sensor);
    Serial.println(" Minor Over ");
    return 2;
  } else if(temp <= 39) {     // MnUn
    Serial.print("Sensor ");
    Serial.print(sensor);
    Serial.println(" Minor Under ");
    return 3;
  } else if(temp <= 49) {     // MjUn
    Serial.print("Sensor ");
    Serial.print(sensor);
    Serial.println(" Major Under ");
    return 4;
  } else {
    return 0;
  }
}

// increment tail, head and loop back to 0 when appropriate
void adjPtrs(){
      if (tTail >= 195){
        tTail = 100;
      } else {
        tTail++;
      }
      if (tTail == tHead){
        tHead++;
        storeTime(headTime);
      }
      if (tHead >= 195){
        tHead = 100;
        storeTime(headTime);
      }
}

void printData(byte stat) { // print values from tail to head
  int start = tTail;
  int limit = 0;
  int head = tHead;
  start -= 1;
  int index = start;
  float current;

  while(index != (head - 1)) {
    if(index < limit) {
      index = 95;
    }
    current = EEPROM.read(index);
    Serial.print(index);
    Serial.print(": ");
    Serial.println(current);
    index--;
  }
  if(tHead != 100) {
    current = EEPROM.read(index);
    Serial.print(index);
    Serial.print(": ");
    Serial.println(current);
  }
}

void recordTemp(float temp){
  static unsigned long int dly;
  int timeCheck;
  switch(chvec){
    case 0:
      EEPROM.write(tTail, temp);
      
//      Serial.print(hum2);
//      Serial.print(" stored at ");
//      Serial.println(hTail);
//      Serial.print(temp2);
//      Serial.print(" stored at ");
//      Serial.println(tTail);
//      Serial.print("hum and temp head ");
//      Serial.print(hHead);
//      Serial.print(" ");
//      Serial.println(tHead);
      
      timeCheck = EEPROM.read(headTime);
      if (timeCheck == 0) { // if no timestamp in EEPROM
        storeTime(headTime);
      }
      adjPtrs();
      dly = millis();
      chvec++;
      break;
    case 1:   
      if ((millis() - dly) > writeDelay) {
        chvec++;
      }
      break;
    case 2:
      if((millis() - dly) > writeDelay) {
        chvec = 0;
      }
      break;
    default:
      chvec = 0;
      break;
 }
}

float * takeReading(){
  static byte vect = 0;
  static unsigned long int dely;
  switch(vect){
    case 0:
      dht22.read2(pinDHT1, &output[0], NULL, NULL);
      dht22.read2(pinDHT2, &output[1], NULL, NULL);
      output[0] = (1.8 * output[0]) + 32;          // convert from nonsense units
      output[1] = (1.8 * output[1]) + 32;          // to Freedom units
      dely = millis();
      vect++;
      return output;
      break;
    case 1:   
      if ((millis() - dely) > queryDelay) {
        dely = millis();
        vect++;
      }
      break;
    case 2:
      if((millis() - dely) > queryDelay) {
        vect = 0;
      }
      break;
    default:
      vect = 0;
      break;
 }
}

