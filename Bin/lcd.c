#ifndef LCD_C
#define LCD_C

/*
  LCD.c
  Takes care of LCD Selection and automatically displaying and update messages.
*/

void waitForPress(){
  while (nLCDButtons == 0){}
  wait1Msec(5);
}

void waitForRelease(){
  while (nLCDButtons != 0){}
  wait1Msec(5);
}

void lcdSelection(){ //Function that handles the side and mode selection during pre auton
  //Clear the LCD
  clearLCDLine(0);
  clearLCDLine(1);

  bLCDBacklight = true;

  //Display filler text
  lcdStr1 = "Select Side:";
  lcdStr2 = "starting ...";
  displayLCDString(0, 0, lcdStr1);
  displayLCDString(1, 0, lcdStr2);

  lcdSideSelection();

  lcdStr2 = "wait for MODE selection ...";
  displayLCDString(1, 0, lcdStr2);
  wait1Msec(250);

  lcdModeSelection();
  bLCDBacklight = false;
}

void lcdSideSelection(){
  //Cycle through sides until the center is pressed
  while (nLCDButtons != CENTERBUTTON){
    if (SIDE == LEFT) lcdStr2 = "LEFT  <  RIGHT";
    if (SIDE == RIGHT) lcdStr2 =  "left  >  RIGHT";
    displayLCDCenteredString(1, lcdStr2);

    waitForPress();
    if (nLCDButtons == LEFTBUTTON){
      SIDE = LEFT;
      waitForRelease();
    }
    if (nLCDButtons == RIGHTBUTTON){
      SIDE = RIGHT;
      waitForRelease();
    }
  }

  if (SIDE == RIGHT) lcdStrSide = "R";
  if (SIDE == LEFT) lcdStrSide = "L";
}

void lcdModeSelection(){
  MODE = AUTO_A;

  lcdStr1 = "Select Mode:";
  displayLCDString(0, 0, lcdStr1);

  //While not selected:
  while (nLCDButtons != CENTERBUTTON){
    //Cycle:
    if (MODE < 0) MODE = 6;
    if (MODE > 5) MODE = 0;
    if (MODE == AUTO_A) lcdStrMode = "Auto A";
    if (MODE == AUTO_B) lcdStrMode = "Auto B";
    if (MODE == AUTO_C) lcdStrMode = "Auto C";
    if (MODE == RBT_SKILL) lcdStrMode = "Rbt Skills";
    if (MODE == PRG_SKILL) lcdStrMode = "Prg Skills";
    if (MODE == TESTING) lcdStrMode = "Testing";
    sprintf(lcdStr2, "<<%s %s>>", lcdStrSide, lcdStrMode);

    //Allow switching between modes
    displayLCDCenteredString(1, lcdStr2);
    waitForPress();
    if (nLCDButtons == LEFTBUTTON){
      waitForRelease();
      MODE--;
    }
    if (nLCDButtons == RIGHTBUTTON){
      waitForRelease();
      MODE++;
    }

  }

  lcdGenerateMessage();
}

void lcdMessage(){
  lcdGenerateMessage();

  displayLCDString(0, 0, lcdStr1);
  displayLCDString(1, 0, lcdStr2);
}


#endif
