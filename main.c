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

#ifndef DEBUG
  #define DEBUG 0
#endif

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
    #if DEBUG == 1 || DEBUG_REMOTE == 1
      writeDebugStreamLine("Setting up remote buttons");
    #endif
    setUpButtons();
    #if DEBUG == 1 || DEBUG_REMOTE == 1
      writeDebugStreamLine("Successfully set up remote buttons");
    #endif
  #endif

  #if USE_SLEW == 1
    #if DEBUG == 1 || DEBUG_SLEW == 1
      writeDebugStreamLine("Slew task is enabled");
    #endif
    startTask(MotorSlewRateTask);
  #else
    #if DEBUG == 1 || DEBUG_SLEW == 1
      writeDebugStreamLine("Slew task is disabled");
    #endif
    startTask(MotorsTask);
  #endif

  #if USE_MONITOR == 1
    startTask(MotorMonitor);
  #endif

  #if USE_FPS == 1
    startTask(fpsTask);
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
  #endif

  #ifdef BAILOUT_BUTTON
    if(vexRT[BAILOUT_BUTTON] == 1){
      #if DEBUG == 1 || DEBUG_REMOTE == 1
        writeDebugStreamLine("Bailout button pressed");
      #endif

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
