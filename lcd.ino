#include <LiquidCrystal.h>

// LCD setup
const int rs = 3, en = 4, d4 = 8, d5 = 7, d6 = 6, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void lcdStart(){
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("I'm alive!"); 
}

void lcdClear(){
  lcd.clear();
}

void lcdPrintAddr(byte ip[]){
  lcd.setCursor(0, 0);
  lcd.print("IP: "); 
  lcd.print(ip[0]);
  lcd.print(".");
  lcd.print(ip[1]);
  lcd.print(".");
  lcd.print(ip[2]);
  lcd.print(".");
  lcd.print(ip[3]);
}

void lcdPrintTraffic(int tX, int rX){
  lcd.setCursor(0, 1);
  lcd.print("Tx: "); 
  lcd.print(tX);
  lcd.print("   Rx: ");
  lcd.print(rX);
}

void lcdPrintTemp(float temp1, float temp2){
  lcd.setCursor(0, 0);
  lcd.print("Int Temp: "); 
  lcd.print(temp1); 
  lcd.setCursor(0, 1);
  lcd.print("Ext Temp: "); 
  lcd.print(temp2); 
}

