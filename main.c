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
  
  if(USE_REMOTE == 1){
    set_up_buttons();
    start_pr_button();
  }
}

void autonProcedure(){
  stopTask(usercontrol);
	clearTimer(T1);

	if (MODE == AUTO_A) auto_A();
  else if (MODE == AUTO_B) auto_B();
	else if (MODE == AUTO_C) auto_C();
	else if (MODE == PRG_SKILL) prg_skills();

	lcd_message();
}

#endif
