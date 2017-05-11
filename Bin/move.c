#ifndef MOVE_C
#define MOVE_C

#define ROTATE_BRAKE 20
#define ROTATE_BRAKE_TIME 60

/*
  BASIC MOVEMENT FUNCTIONS
  These just turn the motors on to accomplish a certain task. There is no control aspect.
*/

void moveStop(){
  move(0, 0, 0);
}

void moveFwd(){
  move(127, 0, 0);
}

void moveBkwd(){
  move(-127, 0, 0);
}

void rotate(int vol){
  move(0, vol, 0);
}

void strafe(int vol){
  move(0, 0, vol);
}

//Allows the robot to move nonlinearly forward
void moveCurve(int level){
  if(level == 0) move(127, 0, 0);
  else if(level == 1) move(127, 15, 0);
  else if(level == 2) move(127, 25, 0);
  else if(level == 3) move(127, 35, 0);
  else if(level == 4) move(127, 45, 0);
  else if(level == 5) move(127, 55, 0);
  else if(level == 6) move(127, 65, 0);
  else if(level == 7) move(127, 75, 0);
  else if(level == 8) move(127, 85, 0);
  else if(level == 9) move(127, 95, 0);
  else if(level == 10) move(127, 127, 0);
  else if(level == -1) move(127, -15, 0);
  else if(level == -2) move(127, -25, 0);
  else if(level == -3) move(127, -35, 0);
  else if(level == -4) move(127, -45, 0);
  else if(level == -5) move(127, -55, 0);
  else if(level == -6) move(127, -65, 0);
  else if(level == -7) move(127, -75, 0);
  else if(level == -8) move(127, -85, 0);
  else if(level == -9) move(127, -95, 0);
  else if(level == -10) move(127, -127, 0);
}

void moveCurveBkwd(int level){
  if(level == 0) move(-127, 0, 0);
  else if(level == 1) move(-127, 15, 0);
  else if(level == 2) move(-127, 25, 0);
  else if(level == 3) move(-127, 35, 0);
  else if(level == 4) move(-127, 45, 0);
  else if(level == 5) move(-127, 55, 0);
  else if(level == 6) move(-127, 65, 0);
  else if(level == 7) move(-127, 75, 0);
  else if(level == 8) move(-127, 85, 0);
  else if(level == 9) move(-127, 95, 0);
  else if(level == 10) move(-127, 127, 0);
  else if(level == -1) move(-127, -15, 0);
  else if(level == -2) move(-127, -25, 0);
  else if(level == -3) move(-127, -35, 0);
  else if(level == -4) move(-127, -45, 0);
  else if(level == -5) move(-127, -55, 0);
  else if(level == -6) move(-127, -65, 0);
  else if(level == -7) move(-127, -75, 0);
  else if(level == -8) move(-127, -85, 0);
  else if(level == -9) move(-127, -95, 0);
  else if(level == -10) move(-127, -127, 0);
}

/*
  CONTROL FUNCTIONS
  Move by a precise amount
*/

void moveBy(int dist, int tlimit){ //Dist in inches, tlimit in milliseconds
  int tnow = time1[T1];
  while(true){
    if(BAILOUT == 1 || isTimedOut(tnow + tlimit) == 1) return;
    else if(fabs(DRV.move[0]) >= fabs(dist)) return;

    wait1Msec(25);
  }
}

void rotateBy(int ang, int tlimit){ //Ang in degrees, tlimit in milliseconds
  int tnow = time1[T1];
  while(true){
    if(BAILOUT == 1 || isTimedOut(tnow + tlimit) == 1) return;
    else if(fabs(DRV.move[2]) >= fabs(ang)) return;

    wait1Msec(25);
  }
}

#endif
