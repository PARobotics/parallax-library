#ifndef FPS_C
#define FPS_C

/*
  FPS.c
  Field positioning system: Tracks the wheel value and gyro value to figure out where the robot is
*/

#if USE_FPS == 1

int fpsGetDegrees(){
  return fps.r;
}

int fpsGetX(){
  return fps.x;
}

int fpsGetY(){
  return fps.y;
}

int fpsSetDegrees(int d){
  fps.r = d;
}

int fpsSetX(int x){
  fps.x = x;
}

int fpsSetY(int y){
  fps.y = y;
}

int fpsDebugCnt = 0;

task fpsTask(){
  if(debug.debug || debug.fps) writeDebugStreamLine("Starting fpsTask");

  int lastLeft = 0;
  int lastRight = 0;
  int lastGyro = 0;

  int currentX, currentY, currentR;

  while(true){
    if((debug.debug || debug.fps) && fpsDebugCnt == 10){
      writeDebugStreamLine("FPS | x: %d y: %d r: %d", fpsGetX(), fpsGetY(), fpsGetDegrees());
      fpsDebugCnt = 0;
    }

    //Update sensors
    updateSensorValue(&fps.left);
    updateSensorValue(&fps.right);
    updateSensorValue(&fps.gyro);

    currentX = fpsGetX();
    currentY = fpsGetY();
    currentR = fpsGetDegrees();

    currentR += gyro.val - lastGyro;
    if(currentR >= 3600) currentR = 0;
    if(currentR < 0) currentR += 3600;

    fpsSetDegrees(currentR);

    

    if(debug.debug || debug.fps) fpsDebugCnt++;
    wait1Msec(10);
  }
}

#endif
#endif
