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
      return vexRT[BAILOUT_BUTTON] == 1;
  #else
     return false;
  #endif
}

#endif
