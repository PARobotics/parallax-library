#ifndef CONFIG_C
#define CONFIG_C

#define USE_LCD       1 //Disable if having LCD reset problems

//Configure slew rate control
#define USE_SLEW      1 //Recommended, but not necessary

//Configure push release
#define USE_REMOTE    1
#define NUM_PR_BUTTONS  0 //Only include if using remote
void setUpButtons(){ //Only include if using remote

}

//Configure preauton
void preAutonProcedure(){
  //This code runs during preauton
}

#define TEAM_NAME     "8900"

void lcdGenerateMessage(){
  sprintf(lcdStr1, "8900 %4.1fV %4.1fV", volt1() / 1000.0, volt2() / 1000.0);
  sprintf(lcdStr2, "Message goes here");
}

#endif
