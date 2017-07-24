#ifndef LIB_MAIN_C
#define LIB_MAIN_C

/*
  MAIN.C
  Implements and combines other library files
  *** Include this file ***
*/

#include "bin/constants.h"
#include "bin/functions.c"
#include "../config.c" //You write this file
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

  #if USE_PR_BUTTON == 1
    if(debug.debug || debug.remote) writeDebugStreamLine("Setting up remote buttons");
    setUpButtons();
    if(debug.debug || debug.remote) writeDebugStreamLine("Successfully set up remote buttons");
  #endif

  #if USE_SLEW == 1
    if(debug.debug || debug.slew) writeDebugStreamLine("Slew task is enabled");
    startTask(MotorSlewRateTask);
  #else
    if(debug.debug || debug.slew) writeDebugStreamLine("Slew task is disabled.");
    startTask(MotorsTask);
  #endif

  #if USE_MONITOR == 1
    startTask(MotorMonitor);
  #endif

  preAutonProcedure();
}

void autonProcedure(){
  stopTask(usercontrol);
	clearTimer(T1);

	#if USE_LCD == 1
    if (MODE == AUTO_A) autoA();
    else if (MODE == AUTO_B) autoB();
  	else if (MODE == AUTO_C) autoC();
  	else if (MODE == PRG_SKILL) prgSkills();
    lcdMessage();
  #endif
}

void userControlUpdate(){
  #if USE_LCD == 1
    lcdMessage();
  #endif

  #if USE_PR_BUTTON == 1
    updatePrbStatus();
    if(debug.remote == 1 || debug.debug == 1){
      for(int i = 0; i < NUM_PR_BUTTONS; i++) writeDebugStreamLine("Button %d: %d", i, getPrButton(i));
    }
  #endif

	if(debug.slew == 1 || debug.debug == 1){
		for(int i = 0; i < 10; i++) writeDebugStreamLine("Motor %d: %3d", i, motor[i]);
	}

  #ifdef BAILOUT_BUTTON
    if(vexRT[BAILOUT_BUTTON] == 1){
      if(debug.debug || debug.remote) writeDebugStreamLine("Bailout button pressed");

      for(int i = 0; i < 10; i++) motor[i] = 0;
      BAILOUT = 1;
      bailOut();
    }
    else
      BAILOUT = 0;
  #endif

  userControlProcedure();

  wait1Msec(10);
}

#endif
