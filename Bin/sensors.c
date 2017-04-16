#ifndef SENSOR_C
#define SENSOR_C

int getMainBatteryVoltage(){ //Returns voltage of main battery in millivolts
  return nImmediateBatteryLevel;
}

int getSecondBatteryVoltage(){ //Returns voltage of power expander battery in millivolts
  return SensorValue(PWR) * 1000 / 286;
}


#endif
