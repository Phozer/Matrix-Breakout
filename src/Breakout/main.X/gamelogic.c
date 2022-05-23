/*
 * File:   gamelogic.c
 * Author: sven_stocker
 *
 * Created on 6. Mai 2022, 15:27
 */


#include "gamelogic.h"
#include "DIGI_DOT_BOOSTER.h"
#include "SPI.h"
#include <stdbool.h>

#define DOWN            0x00
#define UP              0x01
#define RIGHT           0x00
#define LEFT            0x01
#define NONE            0x02

//Globale Variabeln
char posX1Bar, posX2Bar, posBall;
char verticalDirection, horizontalDirection, angle;
const int ballSpeed = 100;
bool bricks[20];


//Spielinitialisierung
void initializeGame(void){
  initializeBricks();
  initializeBar();
  initializeBall();  
}

//Initialisierung von 20 Bricks
void initializeBricks(void){
  //Alle Bricks auf True setzen
  for(int i = 0; i < 20; i++){
    bricks[i] = true;
  }
  //Zeichnen der orangen Bricks
  for(int i = 176; i < 256; i = i + 8){
    if(i >= 192 && i < 208 || i >= 224 && i < 240){
      booster_setRGB(18, 8, 0);
      for(int k = 0; k < 4; k++){
        booster_setLED(i + 4 + k);
      }
    }
    else{
      booster_setRGB(18, 8, 0);
      for(int k = 0; k < 4; k++){
        booster_setLED(i+k);
      }
    }           
  }
  booster_show();
  //Zeichnen der blauen Bricks
  for(int i = 176; i < 256; i = i + 8){
    if(i >= 176 && i < 192 || i >= 208 && i < 224 || i >= 240){
      booster_setRGB(0, 0, 20);
      for(int k = 0; k < 4; k++){
        booster_setLED(i + 4 + k);
      }
    }
    else{
      booster_setRGB(0, 0, 20);
      for(int k = 0; k < 4; k++){
        booster_setLED(i+k);
      }
    }
  }
  booster_show();
}

//Initialisierung des Bretts
void initializeBar(void){
  booster_setRGB(0, 0, 0);
  booster_setRange(0, 15);
  booster_show();
  posX1Bar = 7;
  posX2Bar = 9;
  booster_setRGB(20, 0, 0);
  booster_setRange(posX1Bar, posX2Bar);  
  booster_show();
}

//Bewegen des Bretts
void barMove(char direction){
  //nach links bewegen
  if(direction == LEFT){
    if(posX2Bar < 15){
        posX1Bar++;
        posX2Bar++;
    }
  }
  //nach rechts bewegen
  else if(direction == RIGHT){
    if(posX1Bar > 0){
        posX1Bar--;
        posX2Bar--; 
    }
  }
  barShow();
}

//Anzeigen der neuen Brett Position
void barShow(void){
  INTCONbits.PEIE = 0;                                   //Peripheral Interrupt ausschalten  
  booster_setRGB(0, 0, 0);
  booster_setRange(0, 15);
  booster_show();
  booster_setRGB(20, 0, 0);
  booster_setRange(posX1Bar, posX2Bar);
  booster_show();
  INTCONbits.PEIE = 1;                                    //Peripheral Interrupt einschalten
}

//Initialiserung des Balles
void initializeBall(void){
  posBall = 120;
  booster_setRGB(0, 20, 0);
  booster_setLED(posBall);
  booster_show();
  verticalDirection = DOWN;
  horizontalDirection = NONE;
}

//Bewegen des Balles
void ballMove(void){
  checkWinCondition();
  if(horizontalDirection == NONE){
    ballMoveStraight();
  }
  else{
    ballMoveDiagonal();
  }
  __delay_ms(ballSpeed);
}

//Gerade Bewegung des Balles
void ballMoveStraight(void){
  //Ball bewegt sich nach oben
  if(posBall < 240 && verticalDirection == UP){    
    ballDeleteOldPos();
    posBall = posBall + 16;
    ballShowNewPos();
    if(posBall >= 240){
      verticalDirection = DOWN;
    }    
  }
  //Ball bewegt sich nach unten
  else if(posBall > 31 && verticalDirection == DOWN){
    ballDeleteOldPos();
    posBall = posBall -16;
    ballShowNewPos();
    if(posBall <= 31){
      collisionDetectorWithBar();
    }
  }
  //Bedingung f�r eine Kollision mit einem Brick
  if(posBall > 159){
      collisionDetectorWithBricks();
  }
}

//Diagonale Bewegung des Balles
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
  if(posBall > 159){
      collisionDetectorWithBricks();
  }  
}
  

//L�schen der alten Position des Balles
void ballDeleteOldPos(void){
  INTCONbits.PEIE = 0;                                   //Peripheral Interrupt ausschalten
  booster_setRGB(0,0,0);
  booster_setLED(posBall);
  booster_show();
  INTCONbits.PEIE = 1;                                   //Peripheral Interrupt einschalten
}

void ballShowNewPos(void){
  INTCONbits.PEIE = 0;                                   //Peripheral Interrupt ausschalten
  booster_setRGB(0, 20, 0);
  booster_setLED(posBall);
  booster_show();
  INTCONbits.PEIE = 1;                                   //Peripheral Interrupt einschalten  
}

//Detektion, ob der Ball das Brett trifft
void collisionDetectorWithBar(void){
  verticalDirection = UP;
  //Ball trifft das Brett rechts
  if((posBall - 16) == posX1Bar){
    if(horizontalDirection == NONE){
      horizontalDirection = RIGHT;
    }   
  }
  //Ball trifft das Brett in der Mitte
  else if((posBall - 16) == (posX1Bar + 1)){
    horizontalDirection = NONE;
  }
  //Ball trifft das Brett links
  else if((posBall - 16) == posX2Bar){
    if(horizontalDirection == NONE){
      horizontalDirection = LEFT;
    }
  }
  //Ball trifft das Brett an der rechten Kante
  else if((posBall - 16) == (posX1Bar - 1)){
    horizontalDirection = RIGHT;
  }
  //Ball trifft das Brett an der linken Kante
  else if((posBall - 16) == (posX2Bar + 1)){
    horizontalDirection = LEFT;
  }
  //Ball hat das Brett nicht getroffen
  else{
    INTCONbits.PEIE = 0; 
    initializeBricks();
    ballDeleteOldPos();
    posBall = 120;
    ballShowNewPos();
    horizontalDirection = NONE;
    verticalDirection = DOWN;
    INTCONbits.PEIE = 1; 
  }
}

//Detektion, ob der Ball ein Brick trifft
void collisionDetectorWithBricks(void){
  //Frontales treffen eines Bricks
  if(verticalDirection == UP  && horizontalDirection == NONE){
    if(bricks[(posBall - 160) / 4] == true){
        verticalDirection = DOWN;
        bricks[(posBall - 160) / 4] = false;
        deleteBrick((posBall-160) / 4);
    }    
  }
  //Diagonales Treffen eines Bricks
  else if(verticalDirection == UP && horizontalDirection != NONE){
    if(bricks[(posBall - 160) / 4] == true){
        verticalDirection = DOWN;
        bricks[(posBall - 160) / 4] = false;
        deleteBrick((posBall-160) / 4);    
    }
    if (posBall % 4 == 0){
        if(verticalDirection == UP){
            if(horizontalDirection == RIGHT && bricks[((posBall - 160) / 4) - 4 - 1] == true){
                horizontalDirection = LEFT;
                bricks[((posBall - 160) / 4) - 4 - 1] = false;
                deleteBrick(((posBall - 160) / 4) - 4 - 1);
            }   
            else if(horizontalDirection == RIGHT && bricks[((posBall - 160) / 4) - 1] == true){
                horizontalDirection = LEFT;
                bricks[((posBall - 160) / 4) - 1] = false;
                deleteBrick(((posBall - 160) / 4) - 1);
            }
        }
        else if(verticalDirection == DOWN){
            if(horizontalDirection == RIGHT && bricks[((posBall - 160) / 4) - 1] == true){
                horizontalDirection = LEFT;
                bricks[((posBall - 160) / 4) - 1] = false;
                deleteBrick(((posBall - 160) / 4) - 1);
            }
        }
    }
    if(posBall % 4 == 3){
        if(verticalDirection == UP){
           if(horizontalDirection == LEFT && bricks[((posBall - 160) / 4) - 4 + 1] == true){
                horizontalDirection = RIGHT;
                bricks[((posBall - 160) / 4) - 4 + 1] = false;
                deleteBrick(((posBall - 160) / 4) - 4 + 1);
            }
            else if(horizontalDirection == LEFT && bricks[((posBall - 160) / 4) + 1] == true){
                horizontalDirection = RIGHT;
                bricks[((posBall - 160) / 4) + 1] = false;
                deleteBrick(((posBall - 160) / 4) + 1);
            } 
        }
        else if(verticalDirection == DOWN){
            if(horizontalDirection == LEFT && bricks[((posBall - 160) / 4) + 1] == true){
                horizontalDirection = RIGHT;
                bricks[((posBall - 160) / 4) + 1] = false;
                deleteBrick(((posBall - 160) / 4) + 1);
            }
        }
    }
  }
}

//Brick l�schen bei Aufprall des Balles
void deleteBrick(char arrayIndex){
    INTCONbits.PEIE = 0;                                      //Peripheral Interrupt ausschalten 
    booster_setRGB(0, 0, 0);
    for(int i = 0; i < 4; i ++){
        booster_setLED(176 + i + arrayIndex * 4);
    }
    INTCONbits.PEIE = 1;                                      //Peripheral Interrupt einschalten 
}

//�berpr�fen, ob das Spiel gewonnen wurde
void checkWinCondition(void){
  int count = 0;
  for(int i = 0; i < 20; i++){
      if(bricks[i] == false){
          count++;
      }
  }
  if(count == 20){
      INTCONbits.PEIE = 0;                                    //Peripheral Interrupt ausschalten
      initializeGame();
      INTCONbits.PEIE = 1;                                    //Peripheral Interrupt einschalten
  }
}
