#define keypadIn A1

int reading = 0;              // analog input from keypadIn pin
int keyPressed;               // 1-5 value representing a physical key
int keyDelay = 100;           

// keypad Values
#define BWIN    30        // allowed deviation from input values declared below
#define BLEFT   39        // input from SW1
#define BPLUS   177       // input from SW2
#define BMINUS  349       // input from SW3 
#define BRIGHT  512       // input from SW4
#define BSEL    732       // input from SW5
#define KLEFT   1         // translated input value
#define KPLUS   2         // translated input value
#define KMINUS  3         // translated input value
#define KRIGHT  4         // translated input value
#define KSEL    5         // translated input value
#define BMAX    1023-BWIN 

// convert key inputs
int keyConversion(int key) {
  if (key > BMAX) return 0;
  if (key >= BLEFT - BWIN && key <= BLEFT + BWIN) return KLEFT;
  if (key >= BPLUS - BWIN && key <= BPLUS + BWIN) return KPLUS;
  if (key >= BMINUS - BWIN && key <= BMINUS + BWIN) return KMINUS;
  if (key >= BRIGHT - BWIN && key <= BRIGHT + BWIN) return KRIGHT;
  if (key >= BSEL - BWIN && key <= BSEL + BWIN) return KSEL;
}

int keypad(){
  static byte vect = 0;
  static unsigned long int dly;
  switch(vect){
    case 0:
      reading = analogRead(keypadIn);      // read analog value
      keyPressed = keyConversion(reading);  // convert reading into key pressed value
      vect++;
      break;
    case 1:   
      if ((millis() - dly) > keyDelay) {
        dly = millis();
        vect++;
      }
      break;
    case 2:
      if((millis() - dly) > keyDelay) {
        vect = 0;
      }
      break;
    default:
      vect = 0;
      break;
 }
 return keyPressed;
}



