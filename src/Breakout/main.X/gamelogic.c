/*
 * File:   gamelogic.c
 * Author: sven_stocker
 *
 * Created on 6. Mai 2022, 15:27
 */


#include "gamelogic.h"
#include "DIGI_DOT_BOOSTER.h"
#include "SPI.h"

#define DOWN            0x00
#define UP              0x01
#define RIGHT           0x00
#define LEFT            0x01
#define NONE            0x02
#define LIGHT           0x00
#define STRONG          0x01


//Globale Variabeln
char posX1Bar, posX2Bar, posBall;
char verticalDirection, horizontalDirection, angle;
const int ballSpeed = 120;
char bricks[80];


//Spielinitialisierung

void initalizeGame(void){
  initializeBricks();
  initalizeBar();
  initalizeBall();  
}

void initializeBricks(void){
    char index = 0;
    for(int i = 176; i < 256; i++){
        bricks[index] = i;
        index++;
    }
    for(int i = 176; i < 256; i = i = i++){
        booster_setRGB(0, 0, 20);
        booster_setLED(i);
        booster_show();
    }
}

//Brett-Funktionen
void initalizeBar(void){
  posX1Bar = 7;
  posX2Bar = 9;
  booster_setRGB(20, 0, 0);
  booster_setRange(posX1Bar, posX2Bar);  
  booster_show();
}
void barMove(char direction){         
  if(direction == LEFT){
    if(posX2Bar < 15){
        posX1Bar++;
        posX2Bar++;
    }
  }
  else if(direction == RIGHT){
    if(posX1Bar > 0){
        posX1Bar--;
        posX2Bar--; 
    }
  }
  barShow();
}
void barShow(void){
  booster_setRGB(0, 0, 0);
  booster_setRange(0, 15);
  booster_show();
  booster_setRGB(20, 0, 0);
  booster_setRange(posX1Bar, posX2Bar);
  booster_show(); 
}



//Ball-Funktionen
void initalizeBall(void){
  posBall = 120;
  booster_setRGB(0, 20, 0);
  booster_setLED(posBall);
  booster_show();
  verticalDirection = DOWN;
  horizontalDirection = NONE;
}

void ballMove(void){
  if(horizontalDirection == NONE){
    ballMoveStraight();
  }
  else{
    ballMoveDiagonal();
  }
  __delay_ms(ballSpeed);
}

void ballMoveStraight(void){
  if(posBall < 240 && verticalDirection == UP){    
    ballDeleteOldPos();
    posBall = posBall + 16;
    ballShowNewPos();
    if(posBall >= 240){
      verticalDirection = DOWN;
    }    
  }
  else if(posBall > 31 && verticalDirection == DOWN){
    ballDeleteOldPos();
    posBall = posBall -16;
    ballShowNewPos();
    if(posBall <= 31){
      collisionDetectorWithBar();
    }
  }    
}

void ballMoveDiagonal(void){  
  if(posBall < 240 && verticalDirection == UP && horizontalDirection == LEFT){
    ballDeleteOldPos();
    posBall = posBall + 17;
    ballShowNewPos();
    if(posBall >= 240){
      verticalDirection = DOWN;
    }
    if(posBall % 16 == 15){
      horizontalDirection = RIGHT;
    }
  }
  else if(posBall < 240 && verticalDirection == UP && horizontalDirection == RIGHT){
    ballDeleteOldPos();
    posBall = posBall + 15;
    ballShowNewPos();
    if(posBall >= 240){
      verticalDirection = DOWN;
    }
    if(posBall % 16 == 0){
      horizontalDirection = LEFT;
    }
  }
  else if(posBall > 31 && verticalDirection == DOWN && horizontalDirection == LEFT){
    ballDeleteOldPos();
    posBall = posBall -15;
    ballShowNewPos();
    if(posBall <= 31){
      collisionDetectorWithBar();
    }
    if(posBall % 16 == 15){
      horizontalDirection = RIGHT;
    }
  }
  else if(posBall > 31 && verticalDirection == DOWN && horizontalDirection == RIGHT){
    ballDeleteOldPos();
    posBall = posBall -17;
    ballShowNewPos();
    if(posBall <= 31){
      collisionDetectorWithBar();
    }
    if(posBall % 16 == 0){
      horizontalDirection = LEFT;
    }
  }
}

void ballDeleteOldPos(void){
  booster_setRGB(0,0,0);
  booster_setLED(posBall);
  booster_show();
}

void ballShowNewPos(void){
  booster_setRGB(0, 20, 0);
  booster_setLED(posBall);
  booster_show();  
}

void collisionDetectorWithBar(void){
  verticalDirection = UP;
  if((posBall - 16) == posX1Bar){
    if(horizontalDirection == NONE){
      horizontalDirection = RIGHT;
    }   
  }
  else if((posBall - 16) == (posX1Bar + 1)){
    horizontalDirection = NONE;
  }
  else if((posBall - 16) == posX2Bar){
    if(horizontalDirection == NONE){
      horizontalDirection = LEFT;
    }
  }
  else if((posBall - 16) == (posX1Bar - 1)){
    horizontalDirection = RIGHT;
  }
  else if((posBall - 16) == (posX2Bar + 1)){
    horizontalDirection = LEFT;
  }
  else{
    ballDeleteOldPos();
    posBall = 120;
    ballShowNewPos();
    horizontalDirection = NONE;
    verticalDirection = DOWN;
  }
}

void collisionDetectorWithBricks(void){
    char arrLen = sizeof bricks / sizeof bricks[0];
    for(int i = 0; i < arrLen; i++){
        if(bricks[i] == posBall){
            booster_setRGB(20, 20, 20);
            booster_setLED(posBall);
            booster_show();
        }
    }    
}