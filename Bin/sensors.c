#ifndef SENSOR_C
#define SENSOR_C

#ifndef DEBUG_SENSORS
  #define DEBUG_SENSORS 0
#endif

void sensorReset(){
  for(int i = 0; i < 28; i++){
    SensorValue[i] = 0;
  }
  nMotorEncoder[I2C_1] = 0;
  nMotorEncoder[I2C_2] = 0;
  nMotorEncoder[I2C_3] = 0;
  nMotorEncoder[I2C_4] = 0;
  nMotorEncoder[I2C_5] = 0;
  nMotorEncoder[I2C_6] = 0;
  nMotorEncoder[I2C_7] = 0;
  nMotorEncoder[I2C_8] = 0;
}

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

void initializeSensor(sensor* s, float sF, tSensors p){
  s->scalingFactor = sF;
  s->port = p;
}

void initializeSensor(sensor* s, float sF, tSensors p, pid* PID){
  s->scalingFactor = sF;
  s->port = p;
  s->PID = PID;
}

void updateSensorValue(sensor* s){
  s->valI = s->val;
  s->val = SensorValue(s->port) * s->scalingFactor;

  s->tf = time1[T1];

  int deltaT = s->tf - s->tI;
  if(deltaT == 0) deltaT = 1;

  s->speed = (s->val - s->valI) * 1000 / deltaT;

  s->tI = s->tf;

  #if DEBUG == 1 || DEBUG_SENSORS == 1
    writeDebugStreamLine("[SENSORS] Sensor %d | Value: %d Speed: %d", s->port, s->val, s->speed);
  #endif
}

// ** PID **
int sensorHold(sensor* s, int target, int v_default, int v_min, int v_max){
  int vcmd = v_default - s->PID->kp * (s->val - target) - s->PID->kd * s->speed;
  return BOUND(vcmd, v_min, v_max);
}

int sensorHold(sensor* s, int target, int v_default){
  return sensorHold(s, target, v_default, -127, 127);
}

int sensorPControl(sensor* s, int target){
  int vcmd = s->PID->kp * (target - s->val);

  return BOUND(vcmd, -127, 127);
}

int sensorPDControl(sensor* s, int target, int v_target){
  int vcmd = s->PID->kp * (target - s->val) + s->PID->kd * (v_target - s->speed);

  return BOUND(vcmd, -127, 127);
}

#endif
