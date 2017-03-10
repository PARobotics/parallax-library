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
#include "bin/slew.c"
#include "bin/lcd.c"

void initialize(){

}

void autonProcedure(){
  stopTask(usercontrol);
	clearTimer(T1);

	if (MODE == AUTO_A) auto_A();
	if (MODE == AUTO_B) auto_B();
	if (MODE == AUTO_C) auto_C();
	if (MODE == PRG_SKILL) prg_skills();

	lcd_message();
}

#endif
