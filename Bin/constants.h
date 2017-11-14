#ifndef CONSTANTS_H
#define CONSTANTS_H

#define YES             1
#define NO              0
#define START					  1
#define STOP            0
#define MONITOR         2
#define LEFT           -1
#define RIGHT           1
#define FWD             1
#define BKWD           -1
#define UP              1
#define DOWN           -1
#define MANUAL_UP       2
#define MANUAL_DOWN     3
#define MANUAL 15
#define HOLD            4
#define CARRY           5
#define MOVE            6
#define IN              1
#define OUT            -1
#define OPEN            1
#define CLOSE           -1
#define AUTO_A          0
#define AUTO_B          1
#define AUTO_C          2
#define PRG_SKILL       3
#define RBT_SKILL       4
#define TESTING         5
#define TOGGLE          2
#define CLOCKWISE       1
#define COUNTERCLOCKWISE -1
#define ON 1
#define OFF 0
#define BLINK 3

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
int BAILOUT = 0;

/*
	CONTROL, SENSORS
*/

int motorReq[kNumbOfTotalMotors];     //Array to hold requested speed for the motors

// Constants
#define RPM_393_HS    392      // # ticks per revolution
#define RPM_393       627.2
#define RPM_SE        360

//PID
typedef struct{
	float kp;
	float ki;
	float kd;
} pid;

//Sensors
typedef struct {
  int valI;
  int val;
  int tI;
  int tf;
  int speed;
  float scalingFactor;
  tSensors port;
	pid* PID;
} sensor;

//Motor Monitor
#define M_PI 3.14159265358979323846
int MOVE_MONITOR = STOP;

typedef struct{ //Structure holding all details fpor the drive train
	int x;
	int y;
	int r;
	sensor* left;
	sensor* right;
	sensor* gyro;
	float move[3];
	float degreesToRadians;
} DRIVE;

DRIVE drive;

/*
	Wrapper Functions
*/

//Slew
#define MOTOR_NUM kNumbOfTotalMotors

//Sensors
void sensorReset();
int getMainBatteryVoltage();
int getSecondBatteryVoltage();
void makeLED(tSensors p, int status);
void initializeSensor(sensor* s, float sF, tSensors p);
void initializeSensor(sensor* s, float sF, tSensors p, pid PID);
void updateSensorValue(sensor* s);
int getSensorVCMD(sensor* s, int pe, int se);
int sensorHold(sensor* s, int target, int v_default, int v_min, int v_max);
int sensorHold(sensor* s, int target, int v_default);
int sensorPControl(sensor* s, int target);
int sensorPDControl(sensor* s, int to_target, int v_target);

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
void moveVertical(int vol);
void moveByPID(int dist, int dir, int tlimit);
void strafeByPID(int dist, int dir, int tlimit);
void rotateByPID(int ang, int dir, int tlimit);
void moveStop();
void moveFwd();
void moveBkwd();
void rotate(int vol);
void strafe(int vol);
void moveCurve(int level);
void moveCurveBkwd(int level);
void moveBy(int dist, int tlimit);
void strafeBy(int dist, int tlimit);
void rotateBy(int ang, int tlimit);
void resetDrive(int xNew, int yNew, int rNew);
void refreshDrive();
void initializeDrive(float d2r, sensor* leftS, sensor* rightS, sensor* gyroS);
int driveGetX();
int driveGetY();
int driveGetDegrees();
float driveGetVerticalMovement();
float driveGetLateralMovement();
float driveGetRotationalMovement();

#endif
