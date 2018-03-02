#include <LiquidCrystal.h>

// LCD setup
const int rs = 3, en = 4, d4 = 8, d5 = 7, d6 = 6, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int blinkDly = 800;

void lcdStart(){
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("TempVindicator"); // only stays on screen if something gets stuck in setup()
  lcd.setCursor(0, 1);
  lcd.print("Loading...");
}

void lcdClear(){
  lcd.clear();
}

void lcdBlinkAddr(byte ip[], int idx){
  lcd.setCursor(0, 0);
  lcd.print("IP: "); 
  static byte vec = 0;
  static unsigned long int dly;
  
  switch(vec){
    case 0:
      if(idx == 0){
        lcd.print("  ");
      } else {
        lcd.print(ip[0]);
      }
      lcd.print(".");
      if(idx == 1){
        lcd.print(" ");
      } else {
        lcd.print(ip[1]);
      }
      lcd.print(".");
      if(idx == 2){
        lcd.print(" ");
      } else {
        lcd.print(ip[2]);
      }
      lcd.print(".");
      if(idx == 3){
        lcd.print("   ");
      } else {
        lcd.print(ip[3]);
      }
      lcd.print("    ");
      dly = millis();
      vec++;
      break;
    case 1:   
      if ((millis() - dly) > blinkDly) {
        dly = millis();
        vec++;
      }
      break;
    case 2:
      lcd.print(ip[0]);
      lcd.print(".");
      lcd.print(ip[1]);
      lcd.print(".");
      lcd.print(ip[2]);
      lcd.print(".");
      lcd.print(ip[3]);
      lcd.print("    ");
      if((millis() - dly) > blinkDly) {
        vec = 0;
      }
      break;
    default:
      vec = 0;
      break;
 }
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
  lcd.print("    ");
}

void lcdPrintTraffic(int tX, int rX){
  lcd.setCursor(0, 1);
  if((tX > 999) || (rX > 999)){
    lcd.print("Tx:"); 
    lcd.print(tX);
    lcd.print("  Rx:");
    lcd.print(rX);
    return;
  } else if ((tX > 99) || (rX > 99)){
    lcd.print("Tx: "); 
    lcd.print(tX);
    lcd.print("  Rx: ");
    lcd.print(rX);
    return;
  }
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

