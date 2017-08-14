#ifndef MOVE_C
#define MOVE_C

#ifndef DEBUG_MOVE
  #define DEBUG_MOVE 0
#endif

#if USE_MOVE == 1

void initializeDrive(int x0, int y0, int r, float d2r, sensor* leftS, sensor* rightS, sensor* gyroS){
  drive.x = x0;
  drive.y = y0;
  drive.r = r0;
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
  #if DEBUG == 1|| DEBUG_MOVE == 1
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
        moveDebugCnt = 0;
      #endif
    }
    else if(MOVE_MONITOR == MONITOR){
      #if DEBUG == 1 || DEBUG_MOVE == 1
        if(moveDebugCnt == 10){
          writeDebugStreamLine("[MOVE] %f %f %f", drive.move[0], drive.move[1], drive.move[2]);
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
