#ifndef MOVE_C
#define MOVE_C

#ifndef DEBUG_MOVE
  #define DEBUG_MOVE 0
#endif

#ifndef USE_MOVE
  #define USE_MOVE 0
#endif

#if USE_MOVE == 1

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

void moveCurve(int level){ //Allows the robot to move nonlinearly forward
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
    if(vexRT[BAILOUT_BUTTON] == 1 || isTimedOut(tnow + tlimit) == 1) return;
    else if(fabs(driveGetVerticalMovement()) >= fabs(dist)) return;

    wait1Msec(25);
  }
}

void strafeBy(int dist, int tlimit){ //Dist in inches, tlimit in milliseconds
  int tnow = time1[T1];
  while(true){
    if(vexRT[BAILOUT_BUTTON] == 1 || isTimedOut(tnow + tlimit) == 1) return;
    else if(fabs(driveGetLateralMovement()) >= fabs(dist)) return;

    wait1Msec(25);
  }
}

void rotateBy(int ang, int tlimit){ //Ang in degrees, tlimit in milliseconds
  int tnow = time1[T1];
  while(true){
    if(vexRT[BAILOUT_BUTTON] == 1 || isTimedOut(tnow + tlimit) == 1) return;
    else if(fabs(driveGetRotationalMovement()) >= fabs(ang)) return;

    wait1Msec(25);
  }
}

/*
  MOVE TASK
*/

void initializeDrive(float d2r, sensor* leftS, sensor* rightS, sensor* gyroS){
  drive.left = leftS;
  drive.right = rightS;
  drive.gyro = gyroS;
  drive.degreesToRadians = d2r;
}

void resetDrive(int xNew, int yNew, int rNew){ //Resets coordinates of drive train
  drive.x = xNew;
  drive.y = yNew;
  drive.r = rNew;
}

void refreshDrive(){ //Refreshes drive train to prepare it for another move.
  for(int i = 0; i < 3; i++) drive.move[i] = 0;
}

int driveGetX(){
  return drive.x;
}

int driveGetY(){
  return drive.x;
}

int driveGetDegrees(){
  return drive.r;
}

float driveGetVerticalMovement(){
  return drive.move[0];
}

float driveGetLateralMovement(){
  return drive.move[1];
}

float driveGetRotationalMovement(){
  return drive.move[2];
}

task moveTask(){
  #if DEBUG == 1 || DEBUG_MOVE == 1
    writeDebugStreamLine("Starting moveTask");
    int moveDebugCnt = 0;
  #endif

  float dw_l, dw_r, drot, dd;

  while(true){

    if(MOVE_MONITOR == START){
      updateSensorValue(drive.left);
      updateSensorValue(drive.right);
      updateSensorValue(drive.gyro);

      refreshDrive();

      MOVE_MONITOR = MONITOR;

      #if DEBUG == 1 || DEBUG_MOVE == 1
      	writeDebugStreamLine("Move monitor started");
        moveDebugCnt = 0;
      #endif
    }
    else if(MOVE_MONITOR == MONITOR){
      #if DEBUG == 1 || DEBUG_MOVE == 1
        if(moveDebugCnt == 10){
          writeDebugStreamLine("[MOVE] %f %f %f %d %d %d", drive.move[0], drive.move[1], drive.move[2], drive.x, drive.y, drive.r);
          moveDebugCnt = 0;
        }
      #endif

      updateSensorValue(drive.left);
      updateSensorValue(drive.right);
      updateSensorValue(drive.gyro);

      dw_l = drive.left->val - drive.left->valI;
      dw_r = drive.right->val - drive.right->valI;

      dd = (dw_l + dw_r) / 2.0;

      drot = drive.gyro->val - drive.gyro->valI;

      //Update field position
      drive.x += dd * cos(drive.r * drive.degreesToRadians);
      drive.y += dd * sin(drive.r * drive.degreesToRadians);

      drive.r += drot;
      if(drive.r >= 3600) drive.r %= 3600;
      if(drive.r <= -3600) drive.r = -1 * (abs(drive.r) % 3600);

      //Update move values
      drive.move[0] += dd; //vertical movement
      drive.move[1] += dw_l - dw_r; //lateral movement
      drive.move[2] += drot; //rotational movement

      #if DEBUG == 1 || DEBUG_MOVE == 1
        moveDebugCnt++;
      #endif
    }
    else{
      refreshDrive();
    }

    wait1Msec(10);
  }
}

#endif
#endif
