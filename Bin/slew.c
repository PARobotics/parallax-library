#ifndef SLEW_C
#define SLEW_C

/*
  SLEW.C
  This file implements a slew rate control, so instead of changing motor voltage suddenly, it gradually (but not slowly) changes the voltage.

  USAGE
  In config.c:
  #define USE_REMOTE    1
  #define NUM_PR_BUTTONS  (however many buttons you want to monitor)

  add_pr_button(0, Btn5U) //First parameter is index, second is button to monitor (Do for every button to monitor)

  CHECKING VALUES
  (in user control)
  if(get_pr_button(index) == status){
    //Your code here
    reset_pr_button(index);
  }
  Status can be either PUSHED_RELEASED or LONG_HELD or 0 (Nothing)
*/

// How to use the code from driver code -- example below
// -- initialization
//   int MOTOR_SLEW[10] ={ 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 };  // has to match motor numbers
//   #define MOTOR_SLEW_DELAY        20     //


/*-----------------------------------------------------------------------------*/
/*  definitiona and variables for the motor slew rate controller.              */
// int MOTOR_SLEW[10] ={ 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 };
// #define MOTOR_SLEW_DELAY        15      // task 1/frequency in mS (about 66Hz)
#define MOTOR_NUM               kNumbOfTotalMotors
#define MOTOR_PWM_0           10       //  pwm value when rpm ready to take off from 0
// #define MOTOR_PWM_0           15       //  pwm value when rpm ready to take off from 0
#define MOTOR_PWM_1           90       //  pwm value when rpm almost has peaked
// #define MOTOR_DEADBAND          10
int motorReq [ MOTOR_NUM ];              // Array to hold requested speed for the motors
int BAILOUT = 0;

//----------------------------------------------------------------------------------
int pwmzone(int pwm){
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
task MotorSlewRateTask(){
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
  while( true ){
    // run through motors at each time slice
    for (unsigned  int i=0; i<MOTOR_NUM; i++){
      //

      // add bailout
      if (vexRT[Btn7R] == 1){
        motor[i] = 0;
  			motorReq[i] = 0;
        BAILOUT = 1;
        wait1Msec(100);
        continue;
      }
      else{
        BAILOUT = 0;
      }

      req  = motorReq[i];
      now  = motor   [i];
      // request and current ones are the same, move to next motor
      if (req==now) continue;

      // not using slew control
      if (MOTOR_USE_SLEW != 1){
        motor[i] = req;
      }

      // increase PWM
      else if( req > now ){
        // -- set the now value to upper boundary of the zones
        if      (pwmzone(now) == -2){now = -MOTOR_PWM_1;}    // getting out of zone -1
        else if (pwmzone(now) == 0){now = MOTOR_PWM_0;}      // getting out of zone 0
        else if (pwmzone(now) == 2){now = 127;}              // in zone 1
        // -- do slew rate addtion and set motor PWM
        motor[i] = MIN(req, now+MOTOR_SLEW[i]);
      }

      // decrease PWM
      else if( req < now ){
        // -- set the now value to lower boundary of the zones
        if      (pwmzone(now) == -2){now = -127;}
        else if (pwmzone(now) == 0){now = -MOTOR_PWM_0;}
        else if (pwmzone(now) == 2){now = MOTOR_PWM_1;}
        // -- do slew rate addtion and set motor PWM
        motor[i] = MAX(req, now-MOTOR_SLEW[i]);
      }
       // writeDebugStreamLine("M %4d %2d %4d %4d", time1[T1], i, motorReq[i], motor[i]);
    }


    wait1Msec(50);
  }
}

#endif
