/*
 * File:   gamelogic.c
 * Author: sven_stocker
 *
 * Created on 6. Mai 2022, 15:27
 */


#include "gamelogic.h"
#include "DIGI_DOT_BOOSTER.h"


void initalizeBar(char posX1Bar, char posX2Bar){
  booster_setRGB(20, 0, 0);
  booster_setRange(posX1Bar, posX2Bar);  
  booster_show();
}

void barMove(char posX1Bar, char posX2Bar){
  if (posX1Bar < 13 && posX1Bar >= 0){
    booster_setRGB(0, 0, 0);
    booster_setRange(0, 15);
    booster_show();
    booster_setRGB(20, 0, 0);
    booster_setRange(posX1Bar, posX2Bar);
    booster_show();    
  }  
}

void barChangePosition(char direction){       //direction 1=up, 0 = down
  if(direction == 1){
    if(posX1Bar < 13){
        posX1Bar++;
        posX2Bar++;
    }
  }
  else if(direction == 0){
    if(posX1Bar > 0){
        posX1Bar--;
        posX2Bar--;
    }
  }
}

void initalizeBall(char posBall){
  booster_setRGB(0, 20, 0);
  booster_setLED(posBall);
  booster_show();
}

void ballMoveStraight(void){
  //Logik einfügen
//  if(posBall > 239){
//    
//    booster_setRGB(0,0,0);
//    booster_setLED(posBall);
//    booster_show();
//    posBall = posBall - 16;
//  }
//  else{
//    
//  }
  booster_setRGB(0,0,0);
  booster_setLED(posBall);
  booster_show();
  posBall = posBall + 16;
  booster_setRGB(0, 20, 0);
  booster_setLED(posBall);
  booster_show();
}




