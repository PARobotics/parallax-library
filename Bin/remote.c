#ifndef REMOTE_C
#define REMOTE_C

#if USE_PR_BUTTON == 1

  #define PUSHED_RELEASED 1
  #define LONG_HELD 2

  typedef struct{
    int port;
    int pushed;  // temporary variable to hold that the button has been pushed
    int status;  // 1: yes push and released; 0 no
    int timePushed;
  } PRB_type;

  static PRB_type PRB[NUM_PR_BUTTONS];

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
          if(debug.debug || debug.remote) writeDebugStreamLine("PRB %d has been long held", i);
          PRB[i].status = 2;
        }
        else {
          if(debug.debug || debug.remote) writeDebugStreamLine("PRB %d has been pushed and released", i);
          PRB[i].status = 1;
        }
      }
    }
  }

  void addPrButton(int i, int port){
    PRB[i].port = port;
  }

  int getPrButton(int i){
    return PRB[i].status;
  }

  void resetPrButton(int i){
    PRB[i].status = 0;
  }

#endif

#endif
