#ifndef CONSTANTS_H
#define CONSTANTS_H

#define YES             1
#define NO              0
#define START					  1
#define STOP            0
#define LEFT           -1
#define RIGHT           1
#define FWD             1
#define BKWD           -1
#define UP              1
#define DOWN           -1
#define MANUAL_UP       2
#define MANUAL_DOWN     3
#define HOLD            4
#define CARRY           5
#define IN              1
#define OUT            -1
#define OPEN            1
#define CLOSE           0
#define AUTO_A          0
#define AUTO_B          1
#define AUTO_C          2
#define PRG_SKILL       3
#define RBT_SKILL       4
#define TESTING         5
#define TOGGLE          2
#define CLOCKWISE       1
#define COUNTERCLOCKWISE -1

// LCD
int MODE = AUTO_A;
int SIDE = LEFT;         // Blue or Red side that is selected

string lcdStr1;                // hold the 16 character string for LCD line 1
string lcdStr2;                // hold the 16 character string for LCD line 2
string lcdStrSide;             // 'B' or 'R' indicates the field selection
string lcdStrMode;             // lcdStr for Mode

#define LEFTBUTTON     1
#define CENTERBUTTON   2
#define RIGHTBUTTON    4

//Bailout
int BAILOUT = 1;

//Motor Monitor
#define M_PI 3.14159265358979323846
#define X_POS 0
#define Y_POS 1
#define THETA 2

int WHEEL_MONITOR = 0;
// .1 inch, .,1deg as units
typedef struct {
	int    raw      [3] ;  //raw sensor data [l-wheel, r-wheel, gyro]
	int    raw_last [3] ;  //
	float  pos      [3] ;
	float  move     [3] ;  // -- track dist, lateral and rotation from pre
	float  pos_pre  [3] ;
	float  pos_delta[3] ;  // delta X, Y, R, Rgyro
	float  speed    [2] ;
  int    method       ;  // method for detla_R   1: use gyro, 2: use wheels,
  float  pi2d         ;           // .1 deg per pi
  float  tick2inch    ;   // 0.1 inch per tick
  int    t_last       ;
  int    dbg_cnt      ;
  int    DT           ;
  int    DX           ;               // 12" between the wheel
} drv_train;
drv_train DRV;                      // create drive train data structure

/*
	CONTROL, SENSORS
*/

// Constants
#define RPM_393_HS    392      // # ticks per revolution
#define RPM_393       627.2
#define RPM_SE        360


/*
	Wrapper Functions
*/

//Slew
#define MOTOR_NUM kNumbOfTotalMotors

//Autonomous
void autoA();
void autoB();
void autoC();
void prgSkills();

//Sensors
void sensorReset();
int getMainBatteryVoltage();
int getSecondBatteryVoltage();

//LCD
void waitForPress();
void waitForRelease();
void lcdSelection();
void lcdSideSelection();
void lcdModeSelection();
void lcdMessage();

//Remote
void updatePrbStatus();
void addPrButton(int i, int port);
int getPrButton(int i);
void resetPrButton(int i);

//Bailout
void bailOut();

//Competition Procedure
void userControlProcedure();
void preAutonProcedure();

//Movement
void move(int V, int H, int X);
void moveStop();
void moveFwd();
void moveBkwd();
void rotate(int vol);
void strafe(int vol);
void moveCurve(int level);
void moveCurveBkwd(int level);
void moveBy(int dist, int tlimit);
void rotateBy(int ang, int tlimit);

#endif
