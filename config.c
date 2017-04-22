#ifndef CONFIG_C
#define CONFIG_C

#include "auton.c" //You write this file

#define USE_LCD  0 //Disable if having LCD reset problems, or don't want LCD Selection

//Configure slew rate control
#define USE_SLEW  1 //Disable if slew interferes with move functions or slows robot down
int MOTOR_SLEW[MOTOR_NUM] = {255, 40, 40, 40, 40, 255, 255, 255, 255, 255};

//Configure bailout
#define BAILOUT_BUTTON Btn7R
void bailOut(){

}

//Push release
#define USE_REMOTE    0
#define NUM_PR_BUTTONS  1 //Only include if using remote
void setUpButtons(){ //Only include if using remote
  addPrButton(0, Btn5D); //The number of buttons here MUST be equal to NUM_PR_BUTTONS
}

//Configure preauton
void preAutonProcedure(){
  //This code runs during preauton
}

//Configure LCD
#define TEAM_NAME     "8900"

void lcdGenerateMessage(){
  sprintf(lcdStr1, "8900 %4.1fV %4.1fV", getMainBatteryVoltage() / 1000.0, getSecondBatteryVoltage() / 1000.0);
  sprintf(lcdStr2, "Parallax");
}

//Debug control
#define DEBUG  1
#define DEBUG_SLEW  1

//Sensors
#define USE_SECOND_BATTERY 0

#endif
