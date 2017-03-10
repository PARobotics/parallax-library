#ifndef SLEW_C
#define SLEW_C

/*
  SLEW RATE CONTROL
*/

// MotorCtrl.c implements slewRate control task and use Truespeed for PWM control
// December 2016
//
// How to use the code from driver code -- example below
// -- initialization
//   int MOTOR_SLEW[10] =  { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 };  // has to match motor numbers
//   #define MOTOR_SLEW_DELAY        20     //
//   #include "Libs/MotorCtrl.c"
// -- use it in the code
//  startTask(MotorSlewRateTask,9);
//


/*-----------------------------------------------------------------------------*/
/*  definitiona and variables for the motor slew rate controller.              */
// int MOTOR_SLEW[10] =  { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 };
// #define MOTOR_SLEW_DELAY        15      // task 1/frequency in mS (about 66Hz)
#define MOTOR_NUM               kNumbOfTotalMotors
#define MOTOR_PWM_0           10       //  pwm value when rpm ready to take off from 0
// #define MOTOR_PWM_0           15       //  pwm value when rpm ready to take off from 0
#define MOTOR_PWM_1           90       //  pwm value when rpm almost has peaked
// #define MOTOR_DEADBAND          10
int motorReq [ MOTOR_NUM ];              // Array to hold requested speed for the motors
int BAILOUT = 0;

//----------------------------------------------------------------------------------
int  pwmzone(int pwm)
{
  /*
    seperate the pwm-rpm curve into the following zones to linearing slew rate control
    returns
      0: dead zone 0           [-MOTOR_PWM_0,MOTOR_PWM_0]
      -1: dead zone 1 negative [-127, -MOTOR_PWM_1]
      1: deadzone 1 postive    [MOTOR_PWM_1,127]
      2: normal                [MOTOR_PWM_0 MOTOR_PWM_1]
      -2: normal               [-MOTOR_PWM_1 -MOTOR_PWM_0]
  */
  if (pwm <  -MOTOR_PWM_1) return -2;
  if (pwm <  -MOTOR_PWM_0) return -1;
  if (pwm <=  MOTOR_PWM_0) return 0;
  if (pwm <=  MOTOR_PWM_1) return 1;
  return 2;
}

/*void startSlew(){
  startTask(MotorSlewRateTask);
}*/

//----------------------------------------------------------------------------------
task MotorSlewRateTask()
{
  /*
  MotorSlewRateTask: skips the deadzones to avoid waste time in those zones
  */
  int req, now;         // holder of requested and currect PWM values

  // Initialize
  for ( unsigned  int i=0;i<MOTOR_NUM;i++)
    {
    motorReq[i] = 0;
    // motorSlew[i] = MOTOR_DEFAULT_SLEW_RATE;
    }

  // run task until stopped
  while( true ) {
    // run through motors at each time slice
    for (unsigned  int i=0; i<MOTOR_NUM; i++) {
      //

      // add bailout
      if (vexRT[Btn7R] == 1) {
        motor[i] = 0;
  			motorReq[i] = 0;
        BAILOUT = 1;
        wait1Msec(100);
        continue;
      }
      else {
        BAILOUT = 0;
      }

      req  = motorReq[i];
      now  = motor   [i];
      // request and current ones are the same, move to next motor
      if (req==now) continue;

      // not using slew control
      if (MOTOR_USE_SLEW != 1) {
        motor[i] = req;
      }

      // increase PWM
      else if( req > now ) {
        // -- set the now value to upper boundary of the zones
        if      (pwmzone(now) == -2) {now = -MOTOR_PWM_1;}    // getting out of zone -1
        else if (pwmzone(now) == 0)  {now = MOTOR_PWM_0;}      // getting out of zone 0
        else if (pwmzone(now) == 2)  {now = 127;}              // in zone 1
        // -- do slew rate addtion and set motor PWM
        motor[i] = MIN(req, now+MOTOR_SLEW[i]);
      }

      // decrease PWM
      else if( req < now ) {
        // -- set the now value to lower boundary of the zones
        if      (pwmzone(now) == -2) {now = -127;}
        else if (pwmzone(now) == 0)  {now = -MOTOR_PWM_0;}
        else if (pwmzone(now) == 2)  {now = MOTOR_PWM_1;}
        // -- do slew rate addtion and set motor PWM
        motor[i] = MAX(req, now-MOTOR_SLEW[i]);
      }
       // writeDebugStreamLine("M %4d %2d %4d %4d", time1[T1], i, motorReq[i], motor[i]);
    }

    // Wait approx the speed of motor update over the spi bus

    wait1Msec( MOTOR_SLEW_DELAY );
  }
}

#endif
