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

struct Sensor {
  int valI;
  int valf;
  int tI;
  int tf;
  int speed;
  int (*getSensorValue)()
};

void updateSensorValue(struct Sensor* s){
  s.valI = s.valf;
  s.valf = s.getSensorValue();//somehow get val

  s.tf = time1[T1];

  int deltaT = s.tf - s.ti;
  if(deltaT == 0) deltaT = 1;

  s.speed = (s.valf - s.valI) / deltaT;

  s.tI = s.tF;
}

#endif
