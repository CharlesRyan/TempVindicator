#include <DS3231_Simple.h>
DS3231_Simple Clock;
#include <EEPROM.h>

void rtcStart(){
  Clock.begin();
}

void rtcSet(byte d, byte mo, byte y, byte h, byte mi){
  DateTime MyTimestamp;
  MyTimestamp.Day    = d;
  MyTimestamp.Month  = mo;
  MyTimestamp.Year   = y; 
  MyTimestamp.Hour   = h;
  MyTimestamp.Minute = mi;
  MyTimestamp.Second = 30;
  Clock.write(MyTimestamp);
}

void storeTime(int headTime){
    DateTime theTime = Clock.read();
    EEPROM.write(headTime, theTime.Year);
    EEPROM.write(headTime + 1, theTime.Month);
    EEPROM.write(headTime + 2, theTime.Day);
    EEPROM.write(headTime + 3, theTime.Hour);
    EEPROM.write(headTime + 4, theTime.Minute);
  
//  for(int i = 0; i < 5; i++){ // print timestamp upon write
//    int yargh = EEPROM.read(headTime + i);
//    Serial.println(yargh);
//  }
}
