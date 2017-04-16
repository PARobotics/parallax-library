#ifndef REMOTE_C
#define REMOTE_C

/*
  REMOTE.C
  This file provide enhanced joystick functionality by adding 2 features:
  1) Push Release Buttons: Buttons that execute only once when pressed (perfect for buttons that toggle).
  2) Long hold buttons: Buttons you have to hold for a second to activate.

  USAGE
  In config.c:
  #define USE_REMOTE    1
  #define NUM_PR_BUTTONS  (however many buttons you want to monitor)

  addPrButton(0, Btn5U) //First parameter is index, second is button to monitor (Do for every button to monitor)

  CHECKING VALUES
  (in user control)
  if(getPrButton(index) == status){
    //Your code here
    resetPrButton(index);
  }
  Status can be either PUSHED_RELEASED or LONG_HELD or 0 (Nothing)
*/

#define PUSHED_RELEASED 1
#define LONG_HELD 2

typedef struct{
  int port;
  int pushed;  // temporary variable to hold that the button has been pushed
  int status;  // 1: yes push and released; 0 no
  int timePushed;
} PRB_type;


/*
  PRIVATE INTERFACE
*/

void updatePrbStatus(){
  int sv = 0;     // temp variable to hold sensor value
  for(int i = 0; i < NUM_PR_BUTTONS; i++){
    sv = vexRT[PRB[i].port];

    // When button is pushed
    if (sv == 1){
    	if(PRB[i].pushed == 0) PRB[i].timePushed = time1[T1];
      PRB[i].pushed = 1;
    }
    // Alreay pushed and now released
    if (sv == 0 && PRB[i].pushed == 1){
      PRB[i].pushed = 0;
      if(time1[T1] - PRB[i].timePushed >= 1000){
        PRB[i].status = 2;
      }
      else PRB[i].status = 1;
    }
  }
}

/*
  PUBLIC INTERFACE
*/

void addPrButton(int i, int port);
void startPrButton();
int getPrButton(int i);
void resetPrButton(int i);

#if USE_REMOTE == 1
  static PRB_type PRB[NUM_PR_BUTTONS];

  void addPrButton(int i, int port){
    PRB[i].port = port;
  }

  void startPrButton(){
    startTask(PR_Monitor);
  }

  int getPrButton(int i){
    return PRB[i].status;
  }

  void resetPrButton(int i){
    PRB[i].status = 0;
  }
#endif

task prMonitor(){
  while (true){
    updatePrbStatus();
    wait1Msec(50);
  }
}

#endif
