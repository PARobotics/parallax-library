#ifndef lcd_mode_selection
#define lcd_mode_selection

int MODE = AUTO_A;
int SIDE = LEFT;

string lcdStr1;                // hold the 16 character string for LCD line 1
string lcdStr2;                // hold the 16 character string for LCD line 2
string lcdStrSide;             // 'B' or 'R' indicates the field selection
string lcdStrMode;             // lcdStr for Mode

#define LEFTBUTTON     1
#define CENTERBUTTON   2
#define RIGHTBUTTON    4

void waitForPress(){
  while (nLCDButtons == 0) {}
  wait1Msec(5);
}

void waitForRelease(){
  while (nLCDButtons != 0) {}
  wait1Msec(5);
}

void lcd_selection(){
  // clear the LCD and display filler text
  clearLCDLine(0);
  clearLCDLine(1);

  sprintf(lcdStr1, "8900 %4.1fv", volt1() / 1000.0);
  lcdStr2 =  "starting ...";
  displayLCDString(0, 0, lcdStr1);
  displayLCDString(1, 0, lcdStr2);

  //Wait for center to be pushed
  while (nLCDButtons != CENTERBUTTON) {
    if (SIDE == LEFT) lcdStr2 = "LEFT  <  RIGHT";
    if (SIDE == RIGHT) lcdStr2 =  "left  >  RIGHT";
    displayLCDCenteredString(1, lcdStr2);

    waitForPress();
    if (nLCDButtons == LEFTBUTTON) {
      SIDE = LEFT;
      waitForRelease();
    }
    if (nLCDButtons == RIGHTBUTTON) {
      SIDE = RIGHT;
      waitForRelease();
    }
  }

  if (SIDE == RIGHT) lcdStrSide = "R";
  if (SIDE == LEFT) lcdStrSide = "L";

  lcdStr2 = "wait for MODE selection ...";

  displayLCDString(1, 0, lcdStr2);
  wait1Msec(250);

  MODE = AUTO_A;
  lcd_mode_selection();
}

void lcd_mode_selection(){
  int t_pushed;

  //While not selected:
  while (nLCDButtons != CENTERBUTTON) {
    //Cycle:
    if (MODE < 0) MODE = 6;
    if (MODE > 6) MODE = 0;
    if (MODE == AUTO_A) lcdStrMode = "Auto_A";
    if (MODE == AUTO_B) lcdStrMode = "Auto_B";
    if (MODE == AUTO_C) lcdStrMode = "Auto_C";
    if (MODE == RBT_SKILL) lcdStrMode = "Drv_Skills";
    if (MODE == PRG_SKILL) lcdStrMode = "Prg_Skills";
    if (MODE == TESTING) lcdStrMode = "Testing";
    sprintf(lcdStr2, "<<%s %s>>", lcdStrSide, lcdStrMode);

    // allow switching between modes
    displayLCDCenteredString(1, lcdStr2);
    waitForPress();
    if (nLCDButtons == LEFTBUTTON) {
      waitForRelease();
      MODE--;
    }
    if (nLCDButtons == RIGHTBUTTON) {
      waitForRelease();
      MODE++;
    }

  }

  lcd_msg_init();
}

void lcd_msg_init(){
  sprintf(lcdStr1, "%s %4d", TEAM_NAME, volt1());
  sprintf(lcdStr2, "M%1d %s ", MODE, lcdStrMode);
}

void lcd_message(){
  lcdGenerateMessage();

  displayLCDString(0, 0, lcdStr1);
  displayLCDString(1, 0, lcdStr2);
}


#endif
