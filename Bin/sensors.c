#ifndef SENSOR_C
#define SENSOR_C

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

sensor* initializeSensor(float sF, tSensors p){
  sensor temp;
  temp.scalingFactor = sF;
  temp.port = p;

  pid PID;
  temp.PID = PID;

  return &temp;
}

sensor* initializeSensor(float sF, tSensors p, pid PID){
  sensor temp;
  temp.scalingFactor = sF;
  temp.port = p;
  temp.PID = PID;

  return &temp;
}

void updateSensorValue(sensor* s){
  s->valI = s->val;
  s->val = SensorValue(s->port) * s->scalingFactor;

  s->tf = time1[T1];

  int deltaT = s->tF - s->tI;
  if(deltaT == 0) deltaT = 1;

  s->speed = (s->val - s->valI) / deltaT;

  s->tI = s->tF;

  if(debug.sensors == 1 || debug.debug == 1){
    writeDebugStreamLine("Value: %d Speed: %d", s->val, s->speed);
  }
}

// ** PID **
int sensorHold(sensor* s, int v_default, int target, int v_min, int v_max){
  int vcmd = v_default - s->PID.kp * (s->val - target) - s->PID.kd * s->speed;
  return BOUND(vcmd, v_min, v_max);
}

int sensorHold(sensor* s, int v_default, int target){
  return sensorHold(s, v_default, target, -127, 127);
}



#endif
