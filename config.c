#ifndef CONFIG_C
#define CONFIG_C

#include "auton.c" //You write this file

/*
  COMPETITION PROCEDURE
  Handles all the configuration for the different modes of the game: pre auton, user control, etc.
*/

//Configure preauton
void preAutonProcedure(){
  //This code runs during preauton
}

void userControlProcedure(){
  //This code executes inside the user control while loop
}

/*
  REMOTE
*/

//Set up push release buttons
#define USE_PR_BUTTON  0
#define NUM_PR_BUTTONS  1 //Only include if using remote
void setUpButtons(){ //Only include if using remote
  addPrButton(0, Btn5D); //The number of buttons here MUST be equal to NUM_PR_BUTTONS
}

//Configure bailout
#define BAILOUT_BUTTON Btn7R
void bailOut(){

}

/*
  LCD
*/

#define USE_LCD  0 //Disable if having LCD reset problems, or don't want LCD Selection

void lcdGenerateMessage(){
  sprintf(lcdStr1, "8900 %4.1fV %4.1fV", getMainBatteryVoltage() / 1000.0, getSecondBatteryVoltage() / 1000.0);
  sprintf(lcdStr2, "Parallax");
}

/*
  MOTOR CONTROL
*/

//Slew Rate
#define USE_SLEW  1 //Disable if slew interferes with move functions or slows robot down
int MOTOR_SLEW[MOTOR_NUM] = {255, 40, 40, 40, 40, 255, 255, 255, 255, 255};

//Move functions
void move(int V, int H, int X){
  //V: Forward/Backward motion, H: Rotation, X: Strafe
  //This function is to be filled out so that the robot will move as given by these 3 parameters
}

/*
  SENSORS
*/

//Configure Sensors
#define USE_SECOND_BATTERY 0

/*
  DEBUGGING TOGGLES
*/
#define DEBUG  1
#define DEBUG_SLEW  1
#define DEBUG_REMOTE 0

#endif
