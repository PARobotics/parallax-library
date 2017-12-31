#ifndef FUNCTION_C
#define FUNCTION_C

/*
  Functions.c
  Commonly used functions
*/

// ** Utility Functions **
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define SIGN(X)   ( ((X) > 0) ? (1) : ( ((X) < 0) ? (-1) : (0) ) )
#define BOUND(A,AMIN,AMAX) MIN(AMAX,MAX(A,AMIN))

// ** Exit logic **
bool isTimedOut(int tstop){ // Check if timed out
  return time1[T1] >= tstop;
}

bool isBailedOut(){ // Check if bailed out
  #ifdef BAILOUT_BUTTON
    if (vexRT[BAILOUT_BUTTON] == 1) {
      return true;
    }
    return false;
  #else
     return false;
  #endif
}

// ** Batteries **
int getMainBatteryVoltage(){ //Returns voltage of main battery in millivolts
  return nImmediateBatteryLevel;
}

int getSecondBatteryVoltage(){ //Returns voltage of power expander battery in millivolts
	#ifndef USE_SECOND_BATTERY
	#define USE_SECOND_BATTERY 0
	#endif

	#if USE_SECOND_BATTERY == 1
		return SensorValue(PWR) * 1000 / 286;
	#else
		return 0;
	#endif
}

// ** LED **
void makeLED(tSensors p, int status){
  if(status == TOGGLE) {
    if(SensorValue[p]) SensorValue[p] = false;
    else SensorValue[p] = true;
  }
  else if(status == OFF){
    SensorValue[p] = false;
  }
  else if(status == ON){
    SensorValue[p] = true;
  }
}

#endif
