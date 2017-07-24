// ** Motor and Sensor Setup **

#pragma platform(VEX)
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(1200)
#include "Vex_Competition_Includes.c"

#include "path/to/robotics-library/main.c"

/*
  Pre-autonomous
*/

void pre_auton(){
  bStopTasksBetweenModes = false;
  initialize();
}

/*
 	Autonomous
*/

task autonomous(){
  stopTask(usercontrol);
	autonProcedure();
}

/*
	USER CONTROL
*/

task usercontrol(){
  stopTask(autonomous);

  while(true){
    //put your user control code here

    userControlUpdate();
  }
}
