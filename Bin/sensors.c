#ifndef SENSOR_C
#define SENSOR_C

void sensorReset(){
  for(int i = 0; i < 28; i++){
    SensorValue[i] = 0;
  }
}

int getMainBatteryVoltage(){ //Returns voltage of main battery in millivolts
  return nImmediateBatteryLevel;
}

int getSecondBatteryVoltage(){ //Returns voltage of power expander battery in millivolts
	#if USE_SECOND_BATTERY
		return SensorValue(PWR) * 1000 / 286;
	#else
		return 0;
	#endif
}

typedef struct {
  int valI;
  int val;
  int tI;
  int tf;
  int speed;
  tSensors port;
} sensor;

void updateSensorValue(sensor* s){
  s->valI = s->val;
  s->val = SensorValue(s->port);

  s->tf = time1[T1];

  int deltaT = s->tF - s->tI;
  if(deltaT == 0) deltaT = 1;

  s->speed = (s->val - s->valI) / deltaT;

  s->tI = s->tF;
}

#endif
