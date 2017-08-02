#ifndef MONITOR_C
#define MONITOR_C

#ifndef DEBUG_WHEEL
  #define DEBUG_WHEEL 0
#endif

#define WHEEL_DT  50

int Wheel_t0 = time1[T1];

void wheelInit() {
  for (unsigned int i = 0; i < 3; i++) {
    DRV.raw[i] = 0;
    //DRV.raw_pre  [i] = 0;
    DRV.raw_last[i] = 0;
    DRV.pos[i] = 0.0;
    DRV.pos_pre[i] = 0.0;
    DRV.pos_delta[i] = 0.0;
    DRV.move[i] = 0.0;
  }

  for (unsigned int i = 0; i < 2; i++) {
    DRV.speed[i] = 0;
  }

  DRV.method = 1;                  // method for getting detla_r   1: use gyro, 2: use wheels
  DRV.pi2d = 572.9578;           // .1 deg per pi
  DRV.tick2inch = 4 * 3.14159 / 36;   // 0.1 inch per tick
  DRV.t_last = 0;
  DRV.dbg_cnt = 0;
  DRV.DT = WHEEL_DT;
  DRV.DX = 130;               // 13" between the wheel
}


void wheel_refresh() {
  for (unsigned int i = 0; i < 3; i++) DRV.pos_pre[i] = DRV.pos[i];
  for (unsigned int i = 0; i < 3; i++) DRV.move[i] = 0.0;
}

void wheel_reset(float X, float Y, float R) {
  DRV.pos[X_POS] = X;
  DRV.pos[Y_POS] = Y;
  DRV.pos[THETA] = R;
}

void wheel_output_pos(int lmk_id) {
  if(debug.wheel == 1 || debug.debug == 1){
    writeDebugStreamLine("K%d %4d %4d %4d %4d", lmk_id, time1[T1] - Wheel_t0, DRV.pos[X_POS],DRV.pos[Y_POS],DRV.pos[THETA]);
  }
}

void dbgMsg(const char* msg) {
  if(debug.wheel == 1 || debug.debug == 1){
    writeDebugStreamLine("%% -- %s",msg);
  }
}

/*
  MOTORMONITOR task
  This takes care of updating sensor values
*/

task MotorMonitor(){
  int tnow; int dump_t0; int lift_last_monitor = 0;
  int lift_t0;
  float dw_l,dw_r, drot, dd;
  int dump_dbg_cnt = 0;
  int lift_dbg_cnt = 0;
  while (true) {
    tnow = time1[T1];

    /*
      WHEEL
    */
    if (WHEEL_MONITOR == START){
      getWheelVal();

      //Initialize Values
      Wheel_t0 = tnow;
      DRV.t_last = tnow;
      for(unsigned int i = 0; i < 3; i++) {
        DRV.raw_last[i] = DRV.raw[i];
        DRV.pos_pre[i] = DRV.pos[i];
        DRV.move[i] = 0.0;
      }

      DRV.dbg_cnt  = 0;        // reset debug count
      WHEEL_MONITOR = MONITOR ;         // now set the status to MONITOR
    }
    //Monitor every WHEEL_DT seconds
    else if (WHEEL_MONITOR == MONITOR && tnow >= DRV.t_last + DRV.DT){
      // -- get raw values
      getWheelVal();
      // convert to .1 inch, .1 deg of increment
      dw_l = (DRV.raw[X_POS]-DRV.raw_last[X_POS])*DRV.tick2inch;
      dw_r = (DRV.raw[Y_POS]-DRV.raw_last[Y_POS])*DRV.tick2inch;

      // -- calculate speed and pos (update at DT)
      dd     = (dw_l+dw_r)/2.;

      DRV.pos[X_POS] += dd*cos(DRV.pos[THETA]/DRV.pi2d);
      DRV.pos[Y_POS] += dd*sin(DRV.pos[THETA]/DRV.pi2d);
      DRV.speed[X_POS] = dd*1000/(tnow - DRV.t_last);

      if (DRV.method==2) {
        drot = (dw_l-dw_r)*DRV.pi2d/DRV.DX;
        // DRV.pos[THETA] += (dw_l-dw_r)*DRV.pi2d/DRV.DX;   // method 1
      }
      else if (DRV.method==1) {
        drot = DRV.raw[THETA]-DRV.raw_last[THETA];
        // DRV.pos[THETA] += DRV.raw[THETA]-DRV.raw_last[THETA];    // method 2
      }
      DRV.pos[THETA]   += drot;
      DRV.speed[Y_POS] = drot*1000/(tnow - DRV.t_last);

      // hash lateral (for control purpuse to move straight)
      DRV.move[X_POS] += dd;    // total fwd/bkwd movement
      DRV.move[Y_POS] += dw_l-dw_r;        // total lateral movement
      DRV.move[THETA] += drot;

      // move frame fwd
      DRV.t_last = tnow;
      for (unsigned  int i=0; i<3; i++) DRV.raw_last[i] = DRV.raw[i];
      DRV.dbg_cnt ++;
      if (DEBUG_DRV > 0 && DEBUG_DRV == DRV.dbg_cnt) {
        DRV.dbg_cnt = 0;
        writeDebugStreamLine("W %4d %4d %4d %4d %d", tnow - Wheel_t0, DRV.pos[X_POS],DRV.pos[Y_POS],DRV.pos[THETA], motor[X_POS]);
      }
    }
    else if (WHEEL_MONITOR == STOP){
      if(debug.monitor == 1 || debug.debug == 1){
        writeDebugStreamLine("WHEEL_MONITOR = STOP");
      }
      wheel_refresh();
      DRV.speed[X_POS] = 0;
      DRV.speed[Y_POS] = 0;
    }

    wait1Msec(30);
  }
}


#endif //MONITOR_C
