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

//Control Drive
void move_back_slow_by(int dist, int tlimit) { //
  move_force(-60,0);
  int tnow0 = time1[T1];
  while (true) {
    if (BAILOUT==1) return;
    if (isTimedOut(tnow0 + tlimit) == 1) {
      break;
    }
    if (fabs(DRV.move[0])>=fabs(dist) ) {
      break;
    }
    wait1Msec(2);
  }
  move_force(20,0);
  wait1Msec(30);
  move_stop();
}

//Control Drive
void rotate_slow_by(int ang, int vol, int tlimit) { //
  if (fabs(ang)<=30) return;     // ignore if smaller than 3 degree error
  rotate(-SIGN(ang)*vol); // if positive left rotation
                            // if negative need right rotation to correct, side independent
  // wheel_refresh();    // DRV_move -> 0
  int tnow0 = time1[T1];
  while (true) {
    if (isTimedOut(tnow0 + tlimit) == 1) {
      // writeDebugStream("rotate_by out");
      move_stop();
      return;
    }
    if (fabs(DRV.move[2])>=fabs(ang)) {
      move_force(0,SIGN(ang)*ROTATE_BRAKE);                  // brake at -20
      wait1Msec(ROTATE_BRAKE_TIME);
      move_stop();
      return;
    }
    wait1Msec(2);
  }
}

void rotate_small (int angle) {
  // rotate_small(-300) rotate right for 300 degree
  // used to make small angle adjustment < 20 degree

  wheel_refresh();
  // get the rotating direction set
  int dir = -SIGN(angle);
  // angle = dir*angle;
  angle = fabs(angle);    // abs value of angle

  int vcmd, rpm;
  int tnow0 = time1[T1];
  while (true) {
    if (isTimedOut(tnow0 + 600) == 1) {   // cannot take more than 0.5 sec
      move_stop();
      return;
    }

    rpm = fabs(DRV.speed[1]);      // assuming it takes 80 msec to brake rpm*.04 is about how
                                   // much it will rotate after the brake is started
    if (fabs(DRV.move[2])>angle-rpm/25 ) {  // brake take 0.08 second
      move_force(0,-dir*ROTATE_BRAKE);                  // brake at -20
      wait1Msec(ROTATE_BRAKE_TIME);
      return;
    }

    if (rpm>100) vcmd = -ROTATE_BRAKE;         // apply some break if speed too faster ...
    // if (rpm>1000) vcmd = -ROTATE_BRAKE;         // error in rotate_small

    if (rpm>600) vcmd = 0;         // limit to 60 deg/second
    else vcmd = 40+(600-rpm)*14/100;  // vcmd from 40 to 127 when speed is below 60dps
    move_force(0,dir*vcmd);
    wait1Msec(2);
  }
}


/*
void rotate_brake(int angle, int tlimit) {
//Rotate a certain amount of degrees (Counterclockwise is positive)
  wheel_refresh();
  int tnow0 = time1[T1];
  float R_vel, to_target;
  int dir = -SIGN(angle);
  while (true) {
    if (isTimedOut(tnow0 + tlimit) == 1) {
      // writeDebugStreamLine("Rotate_brake");
      move_stop();
      return;
    }

    R_vel = fabs(DRV.speed[1]);
    to_target = fabs(angle - DRV.move[2]);

    if (to_target > ROTATE_ANG1) {
      ROTATE_VCMD = ROTATE_V1 * dir;
      move_force(0,ROTATE_VCMD);
    }
    else if (to_target > ROTATE_ANG0) {
      ROTATE_VCMD = dir * vcmd_ctrl_linear_band(
                    to_target, R_vel,
                    ROTATE_ANG1, ROTATE_ANG0, ROTATE_Y1, ROTATE_Y2, ROTATE_DY, ROTATE_KV,
                    ROTATE_V_DEF, ROTATE_V_MIN, ROTATE_V_MAX);
      move_force(0,ROTATE_VCMD);
    }
    else {
      ROTATE_VCMD = ROTATE_BRAKE_V * dir;
      move_force(0,ROTATE_VCMD);
      wait1Msec(ROTATE_BRAKE_DT);
      rotate(STOP);
      return;
    }
    wait1Msec(2);
  }
}
*/


void move_brake(int dist, int tlimit) { //Move fwd/backwards by a certain distance

  int dir = SIGN(dist);
  int tnow0 = time1[T1];
  float Y_vel, to_target;

  while (true) {
    if (isTimedOut(tnow0 + tlimit) == 1) {
      move_stop();
      return ;
    }
    to_target = fabs(dist - DRV.move[0]);
    Y_vel = fabs(DRV.speed[0]);

    if (to_target > MOVEY_DIST1) {
      MOVEY_VCMD = MOVEY_V1;
      move_force(dir * MOVEY_VCMD,0);
      //writeDebugStreamLine("%%VCMD = %3d", MOVEY_VCMD);
    }
    else if (to_target > MOVEY_DIST0) {
      MOVEY_VCMD = dir * vcmd_ctrl_linear_band(
                     to_target, Y_vel,
                     MOVEY_DIST1, MOVEY_DIST0, MOVEY_Y1, MOVEY_Y2, MOVEY_DY, MOVEY_KV,
                     MOVEY_V_DEF, MOVEY_V_MIN, MOVEY_V_MAX);
      move_force(MOVEY_VCMD,0);
      //writeDebugStreamLine("%%VCMD2 = %3d", MOVEY_VCMD);
    }
    else {
      MOVEY_VCMD = MOVEY_BRAKE_V * dir;
      move_force(MOVEY_VCMD,0);
      wait1Msec(MOVEY_DT);
      move_stop();
      return;
    }
    wait1Msec(2);
  }
  return;
}

#endif
