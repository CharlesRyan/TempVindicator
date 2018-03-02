bool editMode = false;
int curIdx = 0;
  
void showThresholds(float num1, float num2){
  int status1 = checkLimits(num1, 1);
  int status2 = checkLimits(num2, 2);

  if (editMode == true){
    leds[0] = CRGB(255, 255, 0);
    FastLED.show();
    return;
  }
  if ((status1 == 1) || (status2 == 1)) {            // MjO
    leds[0] = CRGB(255, 0, 0);
    FastLED.show();
  } else if ((status1 == 2) || (status2 == 2)){     // MnO
    leds[0] = CRGB(0, 0, 255);
    FastLED.show();
  } else if ((status1 == 3) || (status2 == 3)){     // MnU
    leds[0] = CRGB(0, 0, 255);
    FastLED.show();
  } else if ((status1 == 4) || (status2 == 4)){     // MjU
    leds[0] = CRGB(255, 0, 0);
    FastLED.show();
  } else if ((status1 == 0) || (status2 == 0)){     // normal
    leds[0] = CRGB(0, 255, 0);
    FastLED.show();
  }
}

void rgbCycle(){  // pretty lights
  for(int i = 0; i < 255; i++){
    FastLED.showColor(CHSV(i++, 255, 255)); 
    delay(10);
  }
}

void menuToggle(){
  if (menu == 0) {
    menu = 1;
  } else if (menu == 1) {
    menu = 0;
  }
}

// constantly being called in loop to update temp
void menuCheck() {
  if (menu == 0) {
    lcdPrintTemp(tempArr[0], tempArr[1]);
  } else if (menu == 1) {
    lcdPrintAddr(getIP());
    lcdPrintTraffic(getTx(), getRx()); 
  }
}

void ipEdit(){
    if(curKey == 2){          // up, increment
      Serial.println("2");
      incIP(curIdx);
    } else if (curKey == 3){  // down, decrement
      Serial.println("3");
      decIP(curIdx);
    } else if (curKey == 1){
      Serial.println("1");
      if(curIdx != 0){        // wrap index
        curIdx--;
      } else {
        curIdx = 3;
      }
    } else if (curKey == 4){
      if(curIdx != 3){
        curIdx++;
      } else {
        curIdx = 0;
      }
    }
}

// changes menu or edits IP depending on button and current menu state
void interface() {
  if(inKey != curKey) {
    lastKey = curKey;
    curKey = inKey;
    if (((curKey == 1) || (curKey == 4)) && !editMode) { // left/right keypress not during IP edit
      lcdClear();
      menuToggle();
    } else if ((curKey == 5) && (menu == 1) && (!editMode)){
      editMode = true;
      ipEdit();
    } else if ((curKey == 5) && (menu == 1) && (editMode)){
      editMode = false;
      lanStart();
    } else if (editMode){
      ipEdit();
    }
  }
  menuCheck();
}
