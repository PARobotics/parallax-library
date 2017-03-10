#ifndef SENSOR_C
#define SENSOR_C

int volt1() { //Returns voltage of main battery in millivolts
  return nImmediateBatteryLevel;
}

int volt2() { //Returns voltage of power expander battery in millivolts
  return SensorValue(PWR) * 1000 / 286;
}


#endif
