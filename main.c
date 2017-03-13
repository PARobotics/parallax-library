#ifndef LIB_MAIN_C
#define LIB_MAIN_C

/*
  MAIN.C
  Implements and combines other library files
  *** Include this file ***
*/

#include "config.c" //You write this file
#include "bin/constants.h"
#include "bin/functions.c"
#include "bin/sensors.c"
#include "bin/remote.c"
#include "bin/slew.c"
#include "bin/lcd.c"

void initialize(){
  bStopTasksBetweenModes = false;
  clearTimer(T1);
  sensor_reset();

  if(USE_LCD == 1){
    lcd_selection();
    lcd_msg_init();
  }

  if(USE_REMOTE == 1){
    set_up_buttons();
    start_pr_button();
  }

  if(USE_SLEW == 1) startTask(MotorSlewRateTask);

  preAutonProcedure();
}

void autonProcedure(){
  stopTask(usercontrol);
	clearTimer(T1);

	if (MODE == AUTO_A) auto_A();
  else if (MODE == AUTO_B) auto_B();
	else if (MODE == AUTO_C) auto_C();
	else if (MODE == PRG_SKILL) prg_skills();

	if(USE_LCD == 1) lcd_message();
}

void userControlUpdate(){
  if(USE_LCD) lcd_message();
  wait1Msec(50);
}

#endif
