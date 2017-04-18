#ifndef LIB_MAIN_C
#define LIB_MAIN_C

/*
  MAIN.C
  Implements and combines other library files
  *** Include this file ***
*/

#include "bin/constants.h"
#include "config.c" //You write this file
#include "bin/functions.c"
#include "bin/sensors.c"
#include "bin/remote.c"
//#include "bin/slew.c"
#include "bin/lcd.c"

void initialize(){
  bStopTasksBetweenModes = false;
  clearTimer(T1);
  sensorReset();

  #if USE_LCD == 1
    lcdSelection();
  #endif

  #if USE_REMOTE == 1
    setUpButtons();
  #endif

  #if USE_SLEW == 1
    startTask(MotorSlewRateTask);
  #endif

  preAutonProcedure();
}

void autonProcedure(){
  stopTask(usercontrol);
	clearTimer(T1);

	if (MODE == AUTO_A) autoA();
  else if (MODE == AUTO_B) autoB();
	else if (MODE == AUTO_C) autoC();
	else if (MODE == PRG_SKILL) prgSkills();

	#if USE_LCD == 1
    lcdMessage();
  #endif
}

void userControlUpdate(){
  #if USE_LCD == 1
    lcdMessage();
  #endif

  #if USE_REMOTE == 1
    updatePrbStatus();
  #endif
  
  wait1Msec(50);
}

#endif
