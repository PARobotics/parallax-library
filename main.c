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
#include "bin/slew.c"
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
  #else
    startTask(MotorsTask);
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

	#if DEBUG == 1
		#if DEBUG_SLEW == 1
			for(int i = 0; i < 10; i++) writeDebugStreamLine("Motor %d: %3d", i, motor[i]);
		#endif

    #if DEBUG_REMOTE == 1
      for(int i = 0; i < NUM_PR_BUTTONS; i++) writeDebugStreamLine("Button %d: %d", i, getPrButton(i));
    #endif
	#endif

  if(vexRT[BAILOUT_BUTTON] == 1){
    BAILOUT = 1;
    bailOut();
  }
  else
    BAILOUT = 0;

  wait1Msec(50);
}

#endif
