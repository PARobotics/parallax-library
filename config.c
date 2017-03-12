#ifndef CONFIG_C
#define CONFIG_C

#define USE_LCD       1 //Disable if having LCD reset problems
#define USE_SLEW      1 //Recommended, but not necessary


//Configure push release
#define USE_REMOTE    1
#define NUM_PR_BUTTONS  0 //Only include if using remote
void set_up_buttons(){ //Only include if using remote

}

#define TEAM_NAME     "8900"



void lcdGenerateMessage(){
  sprintf(lcdStr1, "8900 %4.1fV %4.1fV", volt1() / 1000.0, volt2() / 1000.0);
  sprintf(lcdStr2, "Message goes here");
}

#endif
