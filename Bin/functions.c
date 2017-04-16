#ifndef FUNCTION_C
#define FUNCTION_C

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

int SIGN (int x){
  if (x > 0) return 1;
  if (x < 0) return -1;
  return 0;
}

int BOUND (int a, int amin, int amax){ // Keeps value between amin and amax
  if (a <= amin){a = amin;}
  if (a >= amax){a = amax;}
  return a;
}

int isTimedOut(int tstop){
  if (time1[T1] >= tstop) return 1;
  else return 0;
}

#endif
